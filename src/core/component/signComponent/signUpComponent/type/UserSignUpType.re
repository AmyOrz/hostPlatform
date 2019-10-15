type queryType =
  | UserName
  | Email;

type inputState = {
  logMsg: string,
  isValid: bool,
};

type registerState = {
  logMsg: string,
  isSubmit: bool,
};

type state = {
  userName: string,
  nickName: string,
  password: string,
  email: string,
  userNameInputState: inputState,
  nickNameInputState: inputState,
  emailInputState: inputState,
  passwordInputState: inputState,
  registerState,
};

type action =
  | ChangeUserName(string)
  | ChangeNickName(string)
  | ChangePassword(string)
  | ChangeEmail(string)
  | ChangeUserNameInputState(inputState)
  | ChangeNickNameInputState(inputState)
  | ChangeEmailInputState(inputState)
  | ChangePasswordInputState(inputState)
  | ChangeRegisterState(registerState);