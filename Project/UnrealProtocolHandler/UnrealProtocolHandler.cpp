#include <httplib.h>

#include <format>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Convert a wide Unicode string to an UTF8 string
std::string utf8_encode( const std::wstring& wstr )
{
	if ( !wstr.empty() )
	{
		const int sizeNeeded =
			WideCharToMultiByte( CP_UTF8, 0, wstr.data(), static_cast<int>( wstr.size() ), nullptr, 0, nullptr, nullptr );
		std::string strTo( sizeNeeded, 0 );
		WideCharToMultiByte( CP_UTF8, 0, wstr.data(), static_cast<int>( wstr.size() ), strTo.data(), sizeNeeded, nullptr, nullptr );
		return strTo;
	}
	return {};
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode( const std::string& str )
{
	if ( !str.empty() )
	{
		const int sizeNeeded = MultiByteToWideChar( CP_UTF8, 0, str.data(), static_cast<int>( str.size() ), nullptr, 0 );
		std::wstring wstrTo( sizeNeeded, 0 );
		MultiByteToWideChar( CP_UTF8, 0, str.data(), static_cast<int>( str.size() ), wstrTo.data(), sizeNeeded );
		return wstrTo;
	}
	return {};
}

// @ref https://itc-engineering-blog.netlify.app/blogs/custom-url-msi
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	std::wstring cmd( lpCmdLine );
	std::erase_if( cmd, []( const wchar_t& c ) { return c == '"'; } );	  // 引数が""で囲まれるので削除

	constexpr std::wstring_view header( L"unreal://" );
	if ( const std::wstring::size_type pos = cmd.find( header ); pos != 0 )
	{
		MessageBox( nullptr, std::format( LR"(invalid args: "{}")", cmd ).c_str(), L"error", MB_OK );
		return 0;
	}

	httplib::Client cli( "localhost:30010" );
	if ( const httplib::Result res = cli.Put( "/remote/object/call",
			 json{
				 { "ObjectPath", "/Script/UnrealProtocol.Default__UnrealProtocol" },
				 { "FunctionName", "Entry" },
				 {
					 "Parameters",
					 {
						 { "URL", utf8_encode( cmd.substr( header.size() ) ) },
					 },
				 },
			 }
				 .dump(),
			 "application/json" );
		 !res )
	{
		const std::wstring msg = std::format( LR"(failed to request: "{}")", utf8_decode( to_string( res.error() ) ) );
		MessageBox( nullptr, msg.c_str(), L"error", MB_OK );
	}

	return 0;
}
