type navType =
  | Project
  | Setting
  | Password;

type state = {activeNav: navType};

type action =
  | ChangeNav(navType);

let component = ReasonReact.reducerComponent("UserHomePageComponent");

let reducer = (action, state) =>
  switch (action) {
  | ChangeNav(value) => ReasonReact.Update({...state, activeNav: value})
  };

let render =
    (
      store,
      dispatch,
      languageType,
      {state, send}: ReasonReact.self('a, 'b, 'c),
    ) =>
  <div className="wonder-user-homePage">
    <div className="homePage-content">
      <div className="content-personal">
        <div className="personal-content">
          <img
            src={
              SystemConfig.getServerPath()
              ++ StoreUtils.getUserProfilePath(store)
            }
            className="content-img"
          />
          <div className="content-info">
            <div className="info-userName">
              {DomHelper.textEl(StoreUtils.getUserName(store))}
            </div>
            <div className="info-nickName">
              {DomHelper.textEl(StoreUtils.getUserNickName(store))}
            </div>
          </div>
        </div>
      </div>
      <div className="content-nav">
        <div className="nav-white" />
        <div
          className={
            "nav-item " ++ (state.activeNav === Project ? "nav-active" : "")
          }
          onClick={_e => send(ChangeNav(Project))}>
          <img src="/img/projects.png" />
          <div className="item-text">
            {DomHelper.textEl({j|项目列表|j})}
          </div>
        </div>
        <div
          className={
            "nav-item " ++ (state.activeNav === Setting ? "nav-active" : "")
          }
          onClick={_e => send(ChangeNav(Setting))}>
          <img src="/img/setting.png" />
          <div className="item-text">
            {DomHelper.textEl({j|个人设置|j})}
          </div>
        </div>
        <div
          className={
            "nav-item " ++ (state.activeNav === Password ? "nav-active" : "")
          }
          onClick={_e => send(ChangeNav(Password))}>
          <img src="/img/password.png" />
          <div className="item-text">
            {DomHelper.textEl({j|修改密码|j})}
          </div>
        </div>
      </div>
    </div>
    {
      state.activeNav === Project ?
        <UserProjectComponent store dispatch languageType /> : ReasonReact.null
    }
    {
      state.activeNav === Setting ?
        <UserSettingComponent store dispatch languageType /> : ReasonReact.null
    }
  </div>;

let make = (~store: AppStore.appState, ~dispatch, ~languageType, _children) => {
  ...component,
  initialState: () => {activeNav: Project},
  reducer,
  render: self => render(store, dispatch, languageType, self),
};