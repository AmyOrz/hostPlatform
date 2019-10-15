open LanguageType;

open WonderBsJson;

open WonderBsMost;

type state = {
  isShowUserSignIn: bool,
  isShowUserSignUp: bool,
};

type action =
  | ShowUserSignIn
  | ShowUserSignUp;

let component = ReasonReact.reducerComponent("Header");

let reducer = (action, _state) =>
  switch (action) {
  | ShowUserSignIn =>
    ReasonReact.Update({isShowUserSignIn: true, isShowUserSignUp: false})

  | ShowUserSignUp =>
    ReasonReact.Update({isShowUserSignIn: false, isShowUserSignUp: true})
  };

let render = (languageType, {state, send}: ReasonReact.self('a, 'b, 'c)) =>
  <div className="wonder-userSign">
    <div className="app-bg"> <img src="/img/bg.jpg" /> </div>
    <div className="app-content">
      <div className="content-sign">
        <div className="sign-header">
          <div className="header-item">
            <span
              className={
                "item-content "
                ++ (state.isShowUserSignIn ? "item-active" : "")
              }
              onClick={_e => send(ShowUserSignIn)}>
              {DomHelper.textEl("Sign In")}
            </span>
          </div>
          <div className="header-item"> <img src="/img/logo.png" /> </div>
          <div className="header-item">
            <span
              className={
                "item-content "
                ++ (state.isShowUserSignUp ? "item-active" : "")
              }
              onClick={_e => send(ShowUserSignUp)}>
              {DomHelper.textEl("Sign Up")}
            </span>
          </div>
        </div>
        <div className="sign-content">
          {state.isShowUserSignIn ? <UserSignInComponent /> : ReasonReact.null}
          {state.isShowUserSignUp ? <UserSignUpComponent /> : ReasonReact.null}
        </div>
      </div>
      <div className="content-footer">
        <hr />
        <div className="footer-text">
          {DomHelper.textEl("Wonder Technology")}
        </div>
      </div>
    </div>
  </div>;

let make = (~languageType, _children) => {
  ...component,
  initialState: () => {isShowUserSignIn: true, isShowUserSignUp: false},
  reducer,
  render: self => render(languageType, self),
};