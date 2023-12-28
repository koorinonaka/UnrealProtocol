# UnrealProtocol
Application that executes processing on UnrealEditor from the execution link.
![2023-11-10-12-37-00](https://github.com/koorinonaka/UnrealProtocol/assets/39552085/2c90f257-cdb6-4dda-8d00-5c4c2851f037)

## Installation

### UnrealProtocol.plugin
The root directory is in UnrealEngine plugin format, so place it directly under the `Plugins` directory.

### UnrealProtocol.sln
> [!IMPORTANT]
> To build msi, you need to install [extension](https://itc-engineering-blog.netlify.app/blogs/custom-url-msi#h2-titile-22)

Solution file for application that receive an execution link and relay it to UnrealEditor. The build will generate an installation file.
1. Open `Project/UnrealProtocol.sln`
2. Run `Build Solution`
3. Install `Project/UnrealProtocol/UnrealProtocol.msi`

## Author
[@koorinonaka](https://twitter.com/koorinonaka)

## Documantation
https://qiita.com/koorinonaka/items/d8e9193632125c5dd3d5
