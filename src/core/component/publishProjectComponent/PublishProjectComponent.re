open UserRepoArrayStore;

type navType =
  | Hot
  | New;

type state = {activeNav: navType};

type action =
  | ChangeNav(navType);

let component = ReasonReact.reducerComponent("PublishProjectComponent");

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
  <div className="wonder-publishRepo">
    <Header languageType store dispatch />
    <div className="wonder-publishRepo-main">
      <div className="homePage-content">
        <div className="content-personal">
          <div className="personal-content">
            {DomHelper.textEl({j|公开项目列表|j})}
          </div>
        </div>
        <div className="content-nav">
          <div className="nav-white" />
          <div
            className={
              "nav-item " ++ (state.activeNav === Hot ? "nav-active" : "")
            }
            onClick={_e => send(ChangeNav(Hot))}>
            <img src="/img/hot.png" />
            <div className="item-text">
              {DomHelper.textEl({j|热门项目|j})}
            </div>
          </div>
          <div
            className={
              "nav-item " ++ (state.activeNav === New ? "nav-active" : "")
            }
            onClick={_e => send(ChangeNav(New))}>
            <img src="/img/new.png" />
            <div className="item-text">
              {DomHelper.textEl({j|最新项目|j})}
            </div>
          </div>
        </div>
      </div>
      {
        state.activeNav === Hot ?
          <HotPublishProjectComponent store dispatch languageType /> :
          ReasonReact.null
      }
      {
        state.activeNav === New ?
          <NewPublishProjectComponent store dispatch languageType /> :
          ReasonReact.null
      }
    </div>
  </div>;

let make =
    (~store: AppStore.appState, ~dispatch, ~languageType, ~userId, _children) => {
  ...component,
  initialState: () => {activeNav: Hot},
  reducer,
  render: self =>
    store.isGetUserData ?
      render(store, dispatch, languageType, self) : ReasonReact.null,
  didMount: _self =>
    UserInfoDataUtils.getUserInfoData(userId, dispatch)
    |> WonderBsMost.Most.drain
    |> ignore,
};