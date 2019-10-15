open UserRepoArrayStore;

let component = ReasonReact.statelessComponent("ProjectComponent");

let render = (store, dispatch, languageType, _self) =>
  <div className="wonder-homePage">
    <Header languageType store dispatch />
    <UserTargetProjectComponent
      store
      dispatch
      currentRepo={StoreUtils.unsafeGetCurrentRepo(store)}
      languageType
    />
  </div>;

let make =
    (~store: AppStore.appState, ~dispatch, ~languageType, ~userId, _children) => {
  ...component,
  render: self =>
    store.isGetUserData ?
      render(store, dispatch, languageType, self) : ReasonReact.null,
  didMount: _self => {
    let param = DomHelper.locationSearch(.);
    let repoId = param##repoId;

    switch (repoId |> ValueUtils.isValueValid) {
    | true =>
      UserInfoWithRepoIdUtils.getUserInfoDataWithRepoId(
        userId,
        repoId,
        dispatch,
      )
      |> WonderBsMost.Most.drain
      |> ignore
    | _ => Antd.Message.error({j|参数错误,请联系管理员|j}, 10)
    };
  },
};