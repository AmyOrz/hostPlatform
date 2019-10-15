type inputState = {
  logMsg: string,
  isValid: bool,
};

type state = {
  userName: string,
  firstNewPasssword: string,
  secondNewPasssword: string,
  firstPasswordState: inputState,
  secondPasswordState: inputState,
};

type change =
  | ChangeFirstPassword(string)
  | ChangeSecondPassword(string)
  | ChangeFirstPasswordInputState(inputState)
  | ChangeSecondPasswordInputState(inputState);