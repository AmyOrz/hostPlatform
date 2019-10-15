open Antd;

open WonderBsMost;

open WonderBsJson;

open AppStore;

open UserCurrentRepoStore;

type navType =
  | Overview
  | Setting;

type repoPublish = {useWorker: int};

type state = {
  activeNav: navType,
  repoPublish: option(repoPublish),
  isShowForkModal: bool,
  repoName: string,
};

type action =
  | ChangeNav(navType)
  | ShowForkModal
  | HideForkModal
  | ChangeRepoName(string)
  | ChangeRepoPublish(option(repoPublish));

module Method = {
  let isUserHasThisRepo = (userId, currentRepo) =>
    ValueUtils.isValueEqual(userId, currentRepo.userId);

  let getEditorPathWithRepoId = (userId, hashCode, repoId) =>
    SystemConfig.getEditorPath()
    ++ {j|?userId=$userId&repoId=$repoId&code=$hashCode|j};

  let getRunPathWithRepoId = (repoId, useWorker) =>
    ValueUtils.isValueEqual(useWorker, 1) ?
      SystemConfig.getCurrentLink() ++ {j|/runWithWorker?repoId=$repoId|j} :
      SystemConfig.getCurrentLink() ++ {j|/runWithNoWorker?repoId=$repoId|j};

  let forkTargetRepo = (userId, repoId, state, send) => {
    Js.log(1);

    send(HideForkModal);
  };
};

let component = ReasonReact.reducerComponent("UserTargetProjectComponent");

let reducer = (action, state) =>
  switch (action) {
  | ChangeNav(value) => ReasonReact.Update({...state, activeNav: value})
  | ShowForkModal => ReasonReact.Update({...state, isShowForkModal: true})
  | HideForkModal => ReasonReact.Update({...state, isShowForkModal: false})
  | ChangeRepoPublish(value) =>
    ReasonReact.Update({...state, repoPublish: value})
  | ChangeRepoName(value) => ReasonReact.Update({...state, repoName: value})
  };

let render =
    (
      store,
      dispatch,
      currentRepo,
      languageType,
      {state, send}: ReasonReact.self('a, 'b, 'c),
    ) =>
  <div className="wonder-target-project">
    <div className="project-content">
      <div className="content-repo">
        <div className="repo-content">
          <img
            src={SystemConfig.getServerPath() ++ currentRepo.artPicture}
            className="content-img"
          />
          <div className="content-info">
            <div className="info-name">
              {DomHelper.textEl(currentRepo.name)}
            </div>
            <div className="info-describe">
              {DomHelper.textEl(currentRepo.description)}
            </div>
            <div className="info-operate">
              <div className="operate-editor">
                <a
                  href={
                    Method.getEditorPathWithRepoId(
                      StoreUtils.getUserId(store),
                      StoreUtils.getUserHashCode(store),
                      currentRepo.id,
                    )
                  }
                  target="_blank">
                  <div className="item-text">
                    {DomHelper.textEl({j|编辑器打开|j})}
                  </div>
                  <div className="item-img">
                    <img src="/img/editorRun.png" />
                  </div>
                </a>
              </div>
              {
                switch (state.repoPublish) {
                | None => ReasonReact.null
                | Some(repo) =>
                  <div className="operate-run">
                    <a
                      href={
                        Method.getRunPathWithRepoId(
                          currentRepo.id,
                          repo.useWorker,
                        )
                      }
                      target="_blank">
                      <div className="item-text">
                        {DomHelper.textEl({j|运行|j})}
                      </div>
                      <div className="item-img">
                        <img src="/img/tabRun.png" />
                      </div>
                    </a>
                  </div>
                }
              }
              <div className="operate-views">
                <div className="item-text">
                  <img src="/img/viewWhite.png" />
                  <span className=""> {DomHelper.textEl({j|Views|j})} </span>
                </div>
                <div className="item-count">
                  {DomHelper.intEl(currentRepo.views)}
                </div>
              </div>
              <div
                className="operate-fork" onClick={_e => send(ShowForkModal)}>
                <div className="item-text">
                  <img src="/img/forkWhite.png" />
                  <span className=""> {DomHelper.textEl({j|Fork|j})} </span>
                </div>
                <div className="item-count">
                  {DomHelper.intEl(currentRepo.forks)}
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
      <div className="content-nav">
        <div className="nav-white" />
        <div
          className={
            "nav-item " ++ (state.activeNav === Overview ? "nav-active" : "")
          }
          onClick={_e => send(ChangeNav(Overview))}>
          <img src="/img/projects.png" />
          <div className="item-text">
            {DomHelper.textEl({j|项目概况|j})}
          </div>
        </div>
        {
          Method.isUserHasThisRepo(StoreUtils.getUserId(store), currentRepo) ?
            <div
              className={
                "nav-item "
                ++ (state.activeNav === Setting ? "nav-active" : "")
              }
              onClick={_e => send(ChangeNav(Setting))}>
              <img src="/img/setting.png" />
              <div className="item-text">
                {DomHelper.textEl({j|项目设置|j})}
              </div>
            </div> :
            ReasonReact.null
        }
      </div>
    </div>
    {
      state.isShowForkModal ?
        <Modal
          visible=true
          title={j|Fork 项目|j}
          onOk={
            _e =>
              StringUtils.isValueValid(state.repoName) ?
                Method.forkTargetRepo(currentRepo.id, state, send) :
                Antd.Message.error({j|项目名称不能为空|j}, 4)
          }
          onCancel={_e => send(HideForkModal)}>
          <div>
            {
              DomHelper.textEl(
                {j|克隆别人的项目到自己托管平台，可以在您的项目列表查看该项目。|j},
              )
            }
          </div>
          <Input
            type_="text"
            size="large"
            placeholder={j|请输入项目名称|j}
            value={state.repoName}
            style={"marginTop": 15, "marginBottom": 15}
            onChange={
              event =>
                send(ChangeRepoName(ReactEventType.getFormValue(event)))
            }
          />
        </Modal> :
        ReasonReact.null
    }
    {
      state.activeNav === Overview ?
        <ProjectOverView store dispatch currentRepo languageType /> :
        ReasonReact.null
    }
    {
      state.activeNav === Setting ?
        <ProjectSetting store dispatch currentRepo languageType /> :
        ReasonReact.null
    }
  </div>;

let make =
    (
      ~store: AppStore.appState,
      ~dispatch,
      ~currentRepo,
      ~languageType,
      _children,
    ) => {
  ...component,
  initialState: () => {
    activeNav: Overview,
    repoPublish: None,
    isShowForkModal: false,
    repoName: "",
  },
  reducer,
  render: self => render(store, dispatch, currentRepo, languageType, self),
  didMount: ({send}: ReasonReact.self('a, 'b, 'c)) =>
    UserRepoPublishUtils.getUserRepoPublish(currentRepo.id)
    |> Most.tap(response => {
         Js.log(response##repoPublish);

         switch (response##repoPublish |> ArrayUtils.getFirst) {
         | None => ()
         | Some(repoPublishObj) =>
           send(
             ChangeRepoPublish(
               Some({useWorker: repoPublishObj##use_worker}),
             ),
           )
         };
       })
    |> WonderBsMost.Most.drain
    |> ignore,
};