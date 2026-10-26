# DefaultSkills

프로젝트 종류와 관계없이 작업할 때 따라야 할 기본 작업 규칙입니다.

## 규칙 파일 관리

1. `CLAUDE.md`를 수정할 때는 바꿀 내용을 먼저 보여주고 사용자 승인을 받은 뒤 진행한다.
2. 작업 중 반복해서 지켜야 할 규칙으로 판단되는 내용이 생기면 Claude가 `ClaudeSkills/` 아래 알맞은 Skills 파일에 추가할 수 있다.
3. Skills 파일에 규칙을 추가하거나 수정하면 어떤 파일에 무엇을 추가했는지 항상 사용자에게 알린다.

## 파일 수정

1. 기존 파일을 수정할 때는 원래 인코딩(UTF-8 BOM 유무)과 줄바꿈(CRLF / LF)을 그대로 유지한다.
2. 마크다운 문서에서 한글은 인라인 코드(백틱)로 감싸지 않는다. 그대로 복사해서 실행하는 명령어 코드 블록은 예외다.

## git

1. 커밋과 푸시는 사용자가 요청할 때만 한다.
2. 빌드 산출물과 자동 생성 파일(`.sln`, `.vcxproj`, `Binaries/`, `Intermediate/` 등)은 커밋하지 않는다.

## 언리얼 프로젝트: 코드 수정 후 검증

1. C++ 코드를 수정했으면 엔진의 `Build.bat`으로 에디터 타겟을 빌드해 성공을 확인한다.

```
"<엔진 경로>\Engine\Build\BatchFiles\Build.bat" <프로젝트명>Editor Win64 Development -Project="<.uproject 절대 경로>" -WaitMutex
```

2. 여러 파일에 걸친 수정이나 include를 바꾼 경우에는 `-DisableUnity`를 붙인 빌드도 통과시킨다. 일반(unity) 빌드는 include 누락을 가릴 수 있다.
3. 게임 흐름에 영향을 주는 수정이면 게임을 실행해 런타임 동작을 확인한다. 프로젝트에 자동 테스트용 콘솔 명령이 있으면 그 명령을 `-ExecCmds`로 실행하고, `-abslog`로 남긴 로그를 읽어 검증한 뒤 프로세스를 종료한다.

```
"<엔진 경로>\Engine\Binaries\Win64\UnrealEditor.exe" "<.uproject 절대 경로>" -game -windowed -ResX=960 -ResY=540 -nosound -unattended -ExecCmds="<자동 테스트 명령>" -abslog="<로그 파일 경로>"
```

   - MissionSystem 프로젝트의 자동 테스트 명령은 `ToggleAutoCycleMode`다.
   - 로그만으로 검증했다면 화면(위젯 표시, 카메라 등)은 확인하지 않았다는 점을 사용자에게 함께 보고한다.
4. 블루프린트에 노출된 `UPROPERTY` / `UFUNCTION`의 이름은 바꾸지 않는다. 이름을 바꾸면 블루프린트에 저장된 값이나 노드 연결이 끊어질 수 있다. 꼭 바꿔야 하면 사용자에게 먼저 확인한다.
5. `UENUM` 값을 추가하거나 순서를 바꾸기 전에는 `Content/` 에셋에서 그 enum을 참조하는 곳이 있는지 검색해 확인한다.

## 언리얼 프로젝트: 프로젝트 파일 재생성

저장소 루트에 `.uproject` 파일이 있으면 언리얼 프로젝트로 판단하고, 아래 규칙을 따른다.

1. 다음 중 하나라도 해당하면 Claude가 직접 프로젝트 파일을 재생성한다. 사용자에게 재생성을 요청하지 않는다.
   - `Source/` 아래의 `.h` / `.cpp` 파일을 추가, 삭제, 이름 변경, 이동했을 때
   - `.Build.cs` / `.Target.cs` 파일을 수정했을 때
   - `.uproject`의 모듈 또는 플러그인 목록을 수정했을 때
2. 재생성은 파일 구성 변경 작업이 끝난 직후, 빌드하기 전에 한다.
3. 엔진 경로는 `.uproject`의 `EngineAssociation` 값(예: `5.8`)으로 찾는다. Epic 런처로 설치한 엔진은 `%ProgramData%\Epic\UnrealEngineLauncher\LauncherInstalled.dat`에서 `AppName`이 UE_<버전>인 항목의 `InstallLocation`이 엔진 경로다.
4. 재생성 명령은 엔진의 `Build.bat`을 사용한다.

```
"<엔진 경로>\Engine\Build\BatchFiles\Build.bat" -projectfiles -project="<.uproject 절대 경로>" -game -engine -progress
```

5. `UnrealBuildTool.exe`를 직접 실행하지 않는다. 시스템에 설치된 .NET 버전이 엔진이 요구하는 버전과 달라 실행이 실패할 수 있다. `Build.bat`은 엔진에 포함된 .NET을 사용한다.
6. 출력 마지막의 `Result: Succeeded`로 성공을 확인한다. 실패하면 오류 내용을 사용자에게 그대로 보고한다.
7. 내용이 바뀌지 않은 `.sln` 파일은 다시 쓰이지 않으므로, `.sln`의 수정 시간이 그대로여도 정상이다.
8. 생성된 `.sln` / `.vcxproj` 파일은 git에 add/commit하지 않는다.
