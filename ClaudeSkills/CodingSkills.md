# CodingSkills

코드 작성·수정 시 따르는 코딩 스타일. 규칙에 없는 부분은 주변 코드 스타일을 따른다.

## 1. 언어별 코딩 표준

- 1.1 네이밍, 파일 구성, 언어 고유 문법은 언어 표준을 따른다. C++는 Unreal Engine Coding Standard, C#은 Microsoft C# 코딩 규칙.
- 1.2 언어 표준과 겹치면 이 파일 규칙이 우선이다. 언어별 세부 기준은 13번(C++), 14번(C#).

## 2. 주석

- 2.1 Claude는 주석을 넣지 않는다.
- 2.2 기존 주석은 지우거나 고치지 않는다. 이름을 바꿔서 주석이 틀려지면 먼저 확인하고, 허락받으면 함께 고친다.
- 2.3 클래스 파일 끝에 `}// end class`를 붙인다. 새 파일도 마찬가지.

## 3. 네이밍

- 3.1 약어 없이 의미가 드러나게 쓴다. (`PC` → `PlayerController`)
- 3.2 bool은 `is` / `should` / `show` 등 참·거짓이 드러나는 이름으로 짓는다.
- 3.3 새 enum은 `E`를 붙인다.
- 3.4 `E`가 없는 기존 enum은 요청 전엔 바꾸지 않는다.

## 4. 메서드 이름 패턴

- 4.1 `OnXxx()` — 상태 전환 단계
- 4.2 `OnXxxCompleted()` — 비동기 작업 완료 콜백
- 4.3 `SetXxx()` / `GetXxx()` — 값 설정 / 조회
- 4.4 `IsXxx()` / `ContainsXxx()` — bool 조회
- 4.5 `CheckXxx()` — 조건 확인 후 처리, 처리 여부를 bool로 반환
- 4.6 `LoadXxxUI()` / `ShowXxx()` / `CloseXxxUI()` — UI 열기 / 표시 / 닫기
- 4.7 `RegisterXxx()` / `AddXxx()` / `RemoveXxx()` / `ClearXxx()` — 등록 / 추가 / 제거 / 초기화
- 4.8 `StartXxx()` / `StopXxx()` / `PauseXxx()` / `ResumeXxx()` — 동작 제어
- 4.9 `Xxx_Success()` / `Xxx_Failure()` — 성공 / 실패 분기
- 4.10 Xxx_상황() — 같은 처리를 상황별로 나눔 (`ApplyClearState_Login`)

## 5. 서식

- 5.1 중괄호는 항상 새 줄 (Allman).
- 5.2 한 줄짜리 `if` / `for`도 `{ }`로 감싼다.
- 5.3 `if(`, `else if(`, `for(`는 괄호 앞 공백 없음.
- 5.4 `foreach (`, `switch (`는 괄호 앞 공백 있음.
- 5.5 관련 필드끼리 묶고, 묶음 사이에 빈 줄 하나.
- 5.6 `#region`은 쓰지 않는다.

## 6. 분기

- 6.1 enum 분기는 `if` / `else if` 체인이 기본.
- 6.2 `switch`는 `case` 본문을 `{ }`로 감싸고 `break;`는 중괄호 밖에 둔다.

```cpp
switch (State)
{
    case EState::Idle:
        {
            ...
        }
        break;
}
```

- 6.3 조건이 안 맞으면 함수 앞부분에서 바로 `return`한다.
- 6.4 삼항 연산자는 단순 값 대입에만 쓴다.

## 7. bool 비교

- 7.1 `!` 대신 `== false`를 쓴다.
- 7.2 bool 변수와 bool 반환 함수는 항상 `== true` / `== false`로 비교한다. (`if(TargetPlayer.IsValid() == false)`)

## 8. "시도하고 안 되면 대체 처리" 패턴

- 8.1 할 수도 안 할 수도 있는 처리는 bool을 반환한다.
- 8.2 호출부는 결과가 `== false`일 때 대체 처리한다.

```cpp
if(CheckTimelineAutoPlay(...) == false)
{
    OnAgreeTimelineComplete();
}
```

## 9. 반복문

- 9.1 인덱스 `for`는 전위 증감 `++i` / `--i`.
- 9.2 순회 중 삭제는 역순으로 돈다. (`for(int32 i = List.Num() - 1; i >= 0; --i)`)

## 10. 변수와 타입

- 10.1 타입을 명시한다. (`auto` / `var` 금지)
- 10.2 접근 제한자는 `private`까지 항상 명시한다.
- 10.3 필드는 선언 시 초기값을 명시한다.
- 10.4 null은 `if(x != nullptr)`처럼 명시적으로 비교한다.

## 11. 로그

- 11.1 형식: ## 시스템명 ## 함수명 또는 상황 ## 키 : 값
- 11.2 경고성 로그는 메시지에 `Warning` / `Error`를 넣는다.
- 11.3 진행 불가 실패(데이터 없음, 생성 실패, 중복 등록)는 에러 로그로 남긴다.
- 11.4 상태·설정값을 바꾸는 함수는 바뀐 값을 로그로 남긴다.

## 12. 클래스 구조와 설계

- 12.1 멤버 순서: 중첩 타입 → enum → 상수 → 프로퍼티·필드 → 생명주기 메서드 → 기능별 메서드 → private 헬퍼(파일 끝)
- 12.2 생명주기 메서드는 호출 순서대로 둔다. (`Initialize` → `OnReady` → `OnStart` → `OnProgress` → `OnCompleted` → `Success` / `Fail` → `FinalizeResult`)
- 12.3 외부엔 읽기만 허용하고, 값 변경은 클래스 내부에서만 한다.
- 12.4 상태 변경은 한 함수(`SetXxxState`)에서만 한다.
- 12.5 상태 변경 함수는 성공 여부를 bool로 반환한다.
- 12.6 부모는 자식이 채울 `protected virtual` 빈 함수를 제공한다.
- 12.7 `override`는 부모 함수를 먼저 호출한 뒤 로직을 추가한다.
- 12.8 동작 방식은 enum 필드로 설정한다.
- 12.9 종류별 객체 생성은 enum → 생성 함수 매핑에 등록한다.
- 12.10 같은 종류 중복 등록은 막고 에러 로그를 남긴다.

## 13. C++(언리얼) 적용 기준

- 13.1 파일 끝 표시(2.3)는 헤더 클래스 선언 끝에 `};// end class`. 클래스 없는 헤더와 `.cpp`엔 안 붙인다.
- 13.2 범위 기반 for도 `for(`로 쓴다. (`for(AActor* Actor : FoundActors)`)
- 13.3 한 줄짜리 `FORCEINLINE` getter는 5.1의 예외로 한 줄로 쓴다.
- 13.4 null 확인(10.4)은 조건문 안에서 선언하지 않고, 먼저 선언한 뒤 `if(World == nullptr) { return; }`로 비교한다.
- 13.5 초기값(10.3)은 포인터·숫자·bool·enum 필드에 명시한다. `TArray`, `TWeakObjectPtr`, `FTimerHandle` 등은 생략.
- 13.6 로그는 `UE_LOG`, 진행 불가 실패는 `Error` verbosity. enum은 `*UEnum::GetValueAsString(Value)`, bool은 `*LexToString(bValue)`로 출력한다.
- 13.7 bool 이름(3.2)은 `b` + `Is` / `Should` / `Show` 등. (`bIsEnabled`)

## 14. C# 적용 기준

- 14.1 타입·메서드·프로퍼티·enum 값은 PascalCase, 지역 변수·매개변수는 camelCase, private / protected 필드는 `_camelCase`.
- 14.2 bool 필드는 `_is` / `_should` 접두사. (`_isPaused`)
- 14.3 파일 끝 표시(2.3)는 클래스 닫는 중괄호에 `}// end class`.
- 14.4 타입 명시(10.1)에 따라 `var`와 대상 타입 `new()`를 쓰지 않는다.
- 14.5 null 확인(10.4)은 `if(x != null)`. `?.`는 델리게이트 호출에만 쓴다.
- 14.6 읽기 전용 공개(12.3)는 `{ get; private set; }` 프로퍼티로 한다.
