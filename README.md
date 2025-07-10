# Description
[Jaehuru Game Engine](https://github.com/jaehuru/JaehuruEngine)의 핵심(Core) 모듈입니다.  

## Development Environment
- ' Window OS '
- ' Visual Studio 2022 '

# ⚠️ Caution

## FMOD 라이브러리 관련 설정 안내

**클라이언트 프로젝트 설정**에서 반드시 아래 스크립트를 추가하세요.

### 프로젝트 설정 필수 항목

1. **C/C++ → 일반 → 추가 포함 디렉터리**
```
$(SolutionDir)\JaehuruEngine_Core\External\FMOD\lib\x64
$(SolutionDir)\JaehuruEngine_Core\Engine_Core\Core_SOURCE\public
```
3. **링커 → 일반 → 추가 라이브러리 디렉터리**
```
$(SolutionDir)\JaehuruEngine_Core\External\FMOD\lib\x64
```
4. **링커 → 입력 → 추가 종속성**
```
fmod_vc.lib
fmodstudio_vc.lib
fmodL_vc.lib
fmodstudioL_vc.lib
```
5. **빌드 이벤트 → 빌드 후 이벤트**
```
IF "$(Configuration)"=="Debug" (
    xcopy /Y /D "$(SolutionDir)\JaehuruEngine_Core\External\FMOD\lib\x64\fmodL.dll" "$(OutDir)"
    xcopy /Y /D "$(SolutionDir)\JaehuruEngine_Core\External\FMOD\lib\x64\fmodstudioL.dll" "$(OutDir)"
) ELSE (
    xcopy /Y /D "$(SolutionDir)\JaehuruEngine_Core\External\FMOD\lib\x64\fmod.dll" "$(OutDir)"
    xcopy /Y /D "$(SolutionDir)\JaehuruEngine_Core\External\FMOD\lib\x64\fmodstudio.dll" "$(OutDir)"
)
```
## vcpkg 라이브러리 관리 안내 

이 엔진은 [vcpkg](https://github.com/microsoft/vcpkg)를 사용하여 외부 라이브러리를 관리합니다.

- vcpkg가 시스템에 설치되어 있고, `vcpkg integrate install` 명령으로 Visual Studio와 연동되어 있어야 합니다.
- 프로젝트 루트에 있는 `vcpkg.json` 파일에 필요한 라이브러리 목록이 명시되어 있습니다.
- 다음 명령어로 필요한 라이브러리를 한 번에 설치할 수 있습니다. (엔진 루트 폴더에서 터미널/PowerShell을 열고 실행)
```powershell
vcpkg install
```
---

# About Development

- **개발 중 주요 버그 및 이슈 기록**  
  [🐞 Debug Notes & Issues](https://github.com/jaehuru/JaehuruEngine_Core/issues)

- **Core 엔진 변경 내역 (별도 레포)**  
  [⚙️ CHANGELOG](https://github.com/jaehuru/JaehuruEngine_Core/blob/v2.00/CHANGELOG.md)
  
---

# 참고한 오픈소스

본 프로젝트는 다음 오픈소스를 참고하여 개발되었습니다.

- [YamYam_Engine](https://github.com/eazuooz/YamYam_Engine) (MIT License)  
  일부 구조와 아이디어, 코드를 기반으로 수정 및 확장하여 사용하였습니다.

해당 프로젝트는 MIT 라이선스를 따르며, 원 저작자의 저작권 및 라이선스 정보는 아래와 같습니다:

> 저작권 (MIT License)  
> Copyright (c) 2023  
>  
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:  
>  
> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.  
>  
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
