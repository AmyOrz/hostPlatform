open UserSignUpType;

open WonderBsMost;

open WonderBsJson;

open Antd;

module Method = {
  let isValueHasInValid = state =>
    [|state.userName, state.nickName, state.password, state.email|]
    |> Js.Array.filter(item => item === "")
    |> Js.Array.length >= 1
    || !state.userNameInputState.isValid
    || !state.nickNameInputState.isValid
    || !state.passwordInputState.isValid
    || !state.emailInputState.isValid;

  let submit = (state, send) => {
    let email = state.email;

    Fetch.fetchWithInit(
      SystemConfig.getServerPath() ++ "/registerUser",
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=
          Fetch.BodyInit.make(
            Js.Json.stringify(
              Json.Encode.object_([
                ("userName", Json.Encode.string(state.userName)),
                ("nickName", Json.Encode.string(state.nickName)),
                ("password", Json.Encode.string(state.password)),
                ("email", Json.Encode.string(email)),
                (
                  "targetUrl",
                  Json.Encode.string(SystemConfig.getCurrentLink()),
                ),
              ]),
            ),
          ),
        ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
        (),
      ),
    )
    |> Most.fromPromise
    |> Most.flatMap(response =>
         response |> Fetch.Response.json |> Most.fromPromise
       )
    |> Most.tap(result => {
         let resultObj = JsonType.convertToJsObj(result);
         Js.log(resultObj);

         switch (
           resultObj##status |> UserRouteType.RegisterUserType.convertIntToSelf
         ) {
         | Success =>
           Message.info(
             {j|注册成功，我们已经发送邮件到$email，请进行邮箱验证然后登录|j},
             10,
           );
           send(
             ChangeRegisterState({
               logMsg: {j|注册成功，我们已经发送邮件到$email，请进行邮箱验证后登录|j},
               isSubmit: true,
             }),
           );

         | DBSqlError =>
           send(
             ChangeRegisterState({logMsg: resultObj##msg, isSubmit: true}),
           )
         | InValidParam
         | ParamError =>
           send(
             ChangeRegisterState({
               logMsg: {j|参数错误|j},
               isSubmit: true,
             }),
           )
         | SendEmailError =>
           send(
             ChangeRegisterState({
               logMsg: {j|发送邮件失败, 请联系管理员|j},
               isSubmit: true,
             }),
           )
         };
       })
    |> Most.drain
    |> ignore;
  };
};

let component = ReasonReact.reducerComponent("AddLoginUser");

let reducer = (action, state) =>
  switch (action) {
  | ChangeUserName(value) => ReasonReact.Update({...state, userName: value})
  | ChangeNickName(value) => ReasonReact.Update({...state, nickName: value})
  | ChangePassword(value) => ReasonReact.Update({...state, password: value})
  | ChangeEmail(value) => ReasonReact.Update({...state, email: value})
  | ChangeUserNameInputState(value) =>
    ReasonReact.Update({...state, userNameInputState: value})
  | ChangeNickNameInputState(value) =>
    ReasonReact.Update({...state, nickNameInputState: value})
  | ChangeEmailInputState(value) =>
    ReasonReact.Update({...state, emailInputState: value})
  | ChangePasswordInputState(value) =>
    ReasonReact.Update({...state, passwordInputState: value})
  | ChangeRegisterState(value) =>
    ReasonReact.Update({...state, registerState: value})
  };

let render = ({state, send}: ReasonReact.self('a, 'b, 'c)) =>
  <div className="wonder-signUp">
    {
      state.registerState.logMsg |> StringUtils.isValueValid ?
        <Alert message={state.registerState.logMsg} type_="error" /> :
        ReasonReact.null
    }
    <InputWithLogMsg
      id="userNameInput"
      value={state.userName}
      label="Username"
      onChange={value => send(ChangeUserName(value))}
      isSuccess={state.userNameInputState.isValid}
      successMsg={state.userNameInputState.logMsg}
      failMsg={state.userNameInputState.logMsg}
    />
    <InputWithLogMsg
      id="nickNameInput"
      value={state.nickName}
      label="Nickname"
      onChange={value => send(ChangeNickName(value))}
      isSuccess={state.nickNameInputState.isValid}
      successMsg={state.nickNameInputState.logMsg}
      failMsg={state.nickNameInputState.logMsg}
    />
    <InputWithLogMsg
      id="emailInput"
      value={state.email}
      label="Email"
      onChange={value => send(ChangeEmail(value))}
      isSuccess={state.emailInputState.isValid}
      successMsg={state.emailInputState.logMsg}
      failMsg={state.emailInputState.logMsg}
    />
    <InputWithLogMsg
      id="passwordInput"
      value={state.password}
      type_="password"
      label="Password"
      onChange={value => send(ChangePassword(value))}
      isSuccess={state.passwordInputState.isValid}
      successMsg={state.passwordInputState.logMsg}
      failMsg={state.passwordInputState.logMsg}
    />
    <Button
      className="submit-btn"
      disabled={
        state.registerState.isSubmit ? true : Method.isValueHasInValid(state)
      }
      onClick={_ => Method.submit(state, send)}>
      {DomHelper.textEl("Sign Up")}
    </Button>
  </div>;

let make = _children => {
  ...component,
  initialState: () => {
    userName: "",
    nickName: "",
    password: "",
    email: "",
    userNameInputState: {
      logMsg: {j|用户名，唯一，字母开头，没有空格|j},
      isValid: false,
    },
    nickNameInputState: {
      logMsg: {j|昵称，不唯一，不能少于3位|j},
      isValid: false,
    },
    emailInputState: {
      logMsg: {j|邮箱，需要验证|j},
      isValid: false,
    },
    passwordInputState: {
      logMsg: {j|密码, 不能少于6位|j},
      isValid: false,
    },
    registerState: {
      logMsg: "",
      isSubmit: false,
    },
  },
  reducer,
  didMount: ({send}: ReasonReact.self('a, 'b, 'c)) => {
    UserSignUpUtils.checkFetchByUserInputStream(
      send,
      ("userNameInput", UserName),
      (
        ChangeUserNameInputState({
          logMsg: {j|用户名已经被注册|j},
          isValid: false,
        }),
        ChangeUserNameInputState({logMsg: {j|可以使用|j}, isValid: true}),
      ),
      UserSignUpUtils.isUserNameInputValueValid(send),
    );

    UserSignUpUtils.checkFetchByUserInputStream(
      send,
      ("emailInput", Email),
      (
        ChangeEmailInputState({
          logMsg: {j|此邮箱已经被注册|j},
          isValid: false,
        }),
        ChangeEmailInputState({logMsg: {j|可以使用|j}, isValid: true}),
      ),
      UserSignUpUtils.isEmailInputValueValid(send),
    );

    UserSignUpUtils.checkUserInputStream(
      "nickNameInput",
      UserSignUpUtils.isNickNameInputValueValid(send),
    );

    UserSignUpUtils.checkUserInputStream(
      "passwordInput",
      UserSignUpUtils.isPasswordInputValueValid(send),
    );
  },
  render,
};