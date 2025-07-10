# Changelog

이 프로젝트의 모든 주요 변경 사항은 이 파일에 기록됩니다.

---

# Version 1  
**Win32 API 기반 아키텍처 및 기본 렌더링 구조**

## [V1.00] - 초기 버전

### 윈도우 및 메시지 처리
- WinMain 진입점 및 메시지 루프 구조 구현
- PeekMessage 기반 논블로킹 루프

### 그래픽스 기초
- GDI+를 이용한 이미지 렌더링 및 더블 버퍼링 적용
- 도형 렌더링과 이동을 통해 프레임 기반 구조 이해

### 애플리케이션 구조
- Application 클래스 기반 루프 관리 구조 설계
- 정적/동적 라이브러리 구분 학습

### 게임 오브젝트 및 아키텍처
- GameObject 및 Component 기반 ECS 구조 설계
- Transform, CameraComponent, Layer, SceneManager 구현
- 씬 전환, Instantiate, Destroy, SetActive 기능 도입
- 타일맵 편집기 (ToolScene), 저장/불러오기, 카메라 타겟팅 기능 구현

### 애니메이션 및 렌더링
- Animator / Animation 클래스 및 이벤트 시스템 구축
- 알파 블렌딩 처리 및 BMP 투명도 자동 분기 처리
- CreateAnimationByFolder 기능 구현

### 게임 로직 및 AI
- FSM 상태 전이 및 DeltaTime 기반 AI 시스템 구축

### 물리 및 충돌 처리
- Collider / Rigidbody 구성
- AABB, Circle 등 충돌 판정 구현
- 픽셀 단위 충돌 시스템 포함

### UI 시스템
- UIManager, UIButton, UIHUD 등 구현

### 리소스 및 메모리 관리
- Resource 관리 시스템 구성
- _CrtSetDbgFlag()를 통한 메모리 누수 탐지

### 오디오
- FMOD 연동, AudioListener / AudioSource 구성

### 서브모듈 기반 구조
- 엔진 코드를 Git 서브모듈로 관리
- 정적 라이브러리 기반 아키텍처 정리

---

## [V1.01] - 버그 수정 및 유틸 추가

### Fixed
- CreateAnimationByFolder 함수에서 애니메이션이 하나만 반복 출력되던 문제 수정

### Added
- Vector2 구조체에 Lerp(선형 보간) 함수 추가

---

## [V1.02] - 계층 구조 및 UI 리팩토링

### Added
- GameObject 계층 구조: AddChild, RemoveChild, FindChildOfType 등 구현
- Animator: GetActiveAnimationName, Stop 함수 추가

### Fixed
- 삭제 시점 문제(Dangling Pointer, Iterator Invalidation) 해결

### Changed
- UIManager 리팩토링: Factory 패턴 적용

---

# Version 2  
**DirectX 11 기반 그래픽스 파이프라인 및 엔진 아키텍처 확장**

## [V2.00] - 현재 작업 중
- (작성 예정...)
