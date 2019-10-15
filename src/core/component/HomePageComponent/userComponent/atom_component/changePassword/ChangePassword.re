/* open JustgageReasonCookie;

open ChangePasswordType;

module Method = {
  let isValueHasEmpty = state =>
    state.firstNewPasssword === ""
    || state.secondNewPasssword === ""
    || !state.firstPasswordState.isValid
    || !state.secondPasswordState.isValid;

  let handleSubmit = (state, send) =>
    state.firstNewPasssword === state.secondNewPasssword ?
      send(ChangeFirstPassword("")) :
      {
        send(ChangeFirstPassword(""));
        send(ChangeSecondPassword(""));
        send(
          ChangeFirstPasswordInputState({
            logMsg: {j|两次密码必须相同|j},
            isValid: false,
          }),
        );
        send(
          ChangeSecondPasswordInputState({
            logMsg: {j|两次密码必须相同|j},
            isValid: false,
          }),
        );
      };
};

let component = ReasonReact.reducerComponent("UpdateUserPassword");

let reducer = (action, state) =>
  switch (action) {
  | ChangeFirstPassword(value) =>
    ReasonReact.Update({...state, firstNewPasssword: value})
  | ChangeSecondPassword(value) =>
    ReasonReact.Update({...state, secondNewPasssword: value})
  | ChangeFirstPasswordInputState(value) =>
    ReasonReact.Update({...state, firstPasswordState: value})
  | ChangeSecondPasswordInputState(value) =>
    ReasonReact.Update({...state, secondPasswordState: value})
  };

let render = ({state, send}: ReasonReact.self('a, 'b, 'c)) =>
  <div>
    <h1> {DomHelper.textEl("Change Password")} </h1>
    <FormItem
      value={state.firstNewPasssword}
      logMsg={state.firstPasswordState.logMsg}
      label={j|新密码|j}
      onChange={value => send(ChangeFirstPassword(value))}
      inputId="firstPasswordInput"
      inputType="password"
    />
    <FormItem
      value={state.secondNewPasssword}
      logMsg={state.secondPasswordState.logMsg}
      label={j|确认密码|j}
      onChange={value => send(ChangeSecondPassword(value))}
      inputId="secondPasswordInput"
      inputType="password"
    />
    <button
      disabled={Method.isValueHasEmpty(state)}
      onClick={_ => Method.handleSubmit(state, send)}>
      {{j|提交|j} |> DomHelper.textEl}
    </button>
  </div>;

let make = (~userName, ~code, _children) => {
  ...component,
  initialState: () => {
    userName,
    firstNewPasssword: "",
    secondNewPasssword: "",
    firstPasswordState: {
      logMsg: {j|必填，不能少于6位|j},
      isValid: false,
    },
    secondPasswordState: {
      logMsg: {j|必填，不能少于6位|j},
      isValid: false,
    },
  },
  reducer,
  didMount: ({send}: ReasonReact.self('a, 'b, 'c)) => {
    let handleFirstPasswordInputState = inputState =>
      send(ChangeFirstPasswordInputState(inputState));

    let handleSecondPasswordInputState = inputState =>
      send(ChangeSecondPasswordInputState(inputState));

    UserRegisterUtils.checkUserInputStream(
      "firstPasswordInput",
      ChangePasswordUtils.isPasswordInputValueValid(
        userName,
        handleFirstPasswordInputState,
      ),
    );

    UserRegisterUtils.checkUserInputStream(
      "secondPasswordInput",
      ChangePasswordUtils.isPasswordInputValueValid(
        userName,
        handleSecondPasswordInputState,
      ),
    );
  },
  render,
}; */