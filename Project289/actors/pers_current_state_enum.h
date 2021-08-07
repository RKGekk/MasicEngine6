#pragma once

enum class PersCurrentStateEnum {
	WalkLeft,
	WalkRight,
	WalkToward,
	WalkOutward,
	JumpLeft,
	JumpRight,
	JumpToward,
	JumpOutward,
	IdleLeft,
	IdleRight,
	IdleToward,
	IdleOutward
};

enum class PersCurrentStateClassEnum {
	Walk,
	Jump,
	Idle
};

enum class PersCurrentOrientClassEnum {
	Left,
	Right,
	Toward,
	Outward
};