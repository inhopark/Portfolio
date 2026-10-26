# DefaultSkills

프로젝트와 관계없이 따르는 기본 작업 규칙.

## 1. 규칙 파일 관리

- 1.1 `CLAUDE.md`는 바꿀 내용을 먼저 보여주고 승인받은 뒤 수정한다.
- 1.2 반복해서 지킬 규칙이 생기면 알맞은 Skills 파일에 추가하고, 무엇을 추가했는지 알린다.
- 1.3 규칙으로 만들 만한 요청·지적이 보이면 결과 보고 때 어느 파일에 어떤 문구로 넣을지 제안한다.
- 1.4 Skills 파일은 모두 번호를 붙인다. 섹션은 `## N. 제목`, 규칙은 `N.M`, 하위 항목은 번호 없음.
  - 다른 파일의 규칙은 파일명과 번호로 가리킨다. (예: CodingSkills 10.4)
  - 새 규칙은 섹션 끝에 추가한다. 번호가 바뀌면 그 번호를 가리키는 곳도 고친다.
- 1.5 규칙 추가 요청을 받으면 모든 Skills 파일에서 비슷한 규칙이 있는지 먼저 확인하고, 추가·수정할 문구를 보여준 뒤 승인받고 반영한다.

## 2. 파일 수정

- 2.1 원래 인코딩(BOM 유무)과 줄바꿈(CRLF / LF)을 유지한다.
- 2.2 마크다운에서 한글은 백틱으로 감싸지 않는다. 명령어 코드 블록은 예외.

## 3. git

- 3.1 커밋과 푸시는 사용자가 요청할 때만 한다.
  - "커밋해줘"는 커밋 후 푸시까지 하라는 뜻으로 본다. 커밋만 원하면 사용자가 따로 말한다.
- 3.2 빌드 산출물과 생성 파일(`.sln`, `.vcxproj`, `Binaries/`, `Intermediate/`)은 커밋하지 않는다.

## 4. 언리얼: 코드 수정 후 검증

- 4.1 C++ 수정 후 에디터 타겟을 빌드해 성공을 확인한다.

```
"<엔진 경로>\Engine\Build\BatchFiles\Build.bat" <프로젝트명>Editor Win64 Development -Project="<.uproject 절대 경로>" -WaitMutex
```

- 4.2 여러 파일이나 include를 바꿨으면 `-DisableUnity` 빌드도 통과시킨다. 일반 빌드는 include 누락을 가릴 수 있다.
- 4.3 게임 흐름에 영향이 있으면 자동 테스트 명령(MissionSystem은 `ToggleAutoCycleMode`)을 실행하고 로그로 검증한 뒤 종료한다. 로그만 봤으면 화면은 확인 안 했다고 보고한다.

```
"<엔진 경로>\Engine\Binaries\Win64\UnrealEditor.exe" "<.uproject 절대 경로>" -game -windowed -ResX=960 -ResY=540 -nosound -unattended -ExecCmds="<자동 테스트 명령>" -abslog="<로그 파일 경로>"
```

- 4.4 블루프린트에 노출된 `UPROPERTY` / `UFUNCTION` 이름은 바꾸지 않는다(BP 값·노드 연결이 끊김). 꼭 필요하면 먼저 확인한다.
- 4.5 `UENUM` 값을 추가하거나 순서를 바꾸기 전에 `Content/`에서 참조를 검색한다.

## 5. 언리얼: 프로젝트 파일 재생성

- 5.1 `.uproject`가 있는 프로젝트(하위 폴더 포함)에서 아래 경우 빌드 전에 Claude가 직접 재생성한다.
  - `Source/`의 `.h` / `.cpp` 추가·삭제·이름 변경·이동
  - `.Build.cs` / `.Target.cs` 수정
  - `.uproject`의 모듈·플러그인 목록 수정
- 5.2 엔진 경로는 `.uproject`의 `EngineAssociation`으로 `%ProgramData%\Epic\UnrealEngineLauncher\LauncherInstalled.dat`에서 UE_<버전>의 `InstallLocation`을 찾는다.
- 5.3 재생성은 `Build.bat`으로 한다. `UnrealBuildTool.exe`는 .NET 버전 불일치로 실패할 수 있어 직접 실행하지 않는다.

```
"<엔진 경로>\Engine\Build\BatchFiles\Build.bat" -projectfiles -project="<.uproject 절대 경로>" -game -engine -progress
```

- 5.4 `Result: Succeeded`로 성공을 확인하고, 실패하면 오류를 그대로 보고한다. 내용이 같으면 `.sln`은 다시 쓰이지 않아 수정 시간이 그대로여도 정상이다.

## 6. 작업 마무리

- 6.1 요청받은 작업을 끝내면 결과를 5번 정도 다시 검토한 뒤 결론을 가지고 보고한다.
  - 검토 관점: 요청대로 됐는지, 빠진 것, 틀린 것, 다른 곳에 미치는 영향, 더 나은 방법
