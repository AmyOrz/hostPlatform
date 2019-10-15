open UserRepoArrayStore;

let component = ReasonReact.statelessComponent("HomePageComponent");

let render = (store, dispatch, languageType, _self) =>
  <div className="wonder-homePage">
    <Header languageType store dispatch />
    <UserHomePageComponent store dispatch languageType />
  </div>;

let make =
    (~store: AppStore.appState, ~dispatch, ~languageType, ~userId, _children) => {
  ...component,
  render: self =>
    store.isGetUserData ?
      render(store, dispatch, languageType, self) : ReasonReact.null,
  didMount: _self =>
    UserInfoDataUtils.getUserInfoData(userId, dispatch)
    |> WonderBsMost.Most.drain
    |> ignore,
};