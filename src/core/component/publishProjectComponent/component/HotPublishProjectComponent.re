open PublishRepoType;

module Method = {
  let buildPublishRepoItem = repoArray =>
    repoArray
    |> Js.Array.mapi((repoItem, i) => {
         let repoId = repoItem.repoId;
         let userId = repoItem.userId;

         <div className="repo-item" key={i |> string_of_int}>
           <div className="item-img">
             <a
               href={
                 ValueUtils.isValueEqual(repoItem.useWorker, 1) ?
                   {j|/runWithWorker?repoId=$repoId|j} :
                   {j|/runWithNoWorker?repoId=$repoId|j}
               }
               target="_blank">
               <div className="img-views">
                 <img src="/img/play.png" />
                 <span> {DomHelper.intEl(repoItem.views)} </span>
               </div>
               <img
                 className="img-content"
                 src={SystemConfig.getServerPath() ++ repoItem.artPicture}
               />
             </a>
           </div>
           <div
             className="item-repoName"
             onClick={
               _e =>
                 DomHelper.locationHref({j|targetProject?repoId=$repoId|j})
             }>
             {DomHelper.textEl(repoItem.repoName)}
           </div>
           <div
             className="item-userName"
             onClick={
               _e => DomHelper.locationHref({j|targetUser?userId=$userId|j})
             }>
             {DomHelper.textEl(repoItem.userName)}
           </div>
         </div>;
       });

  let loadHotPublishProject = (state, send) =>
    PublishProjectUtils.getHotPublishProject(
      state.page,
      state.count,
      state.publishRepoArray,
      send,
    )
    |> WonderBsMost.Most.drain
    |> ignore;
};

let component = ReasonReact.reducerComponent("HotPublishProject");

let reducer = (action, state) =>
  switch (action) {
  | ChangePublishRepo(value) =>
    ReasonReact.Update({...state, publishRepoArray: value})
  | ChangePage(value) => ReasonReact.Update({...state, page: value})
  | ChangeIsProjectOver => ReasonReact.Update({...state, isProjectOver: true})
  };

let render =
    (
      store,
      dispatch,
      languageType,
      {state, send}: ReasonReact.self('a, 'b, 'c),
    ) =>
  <div className="wonder-publish-repo">
    <div className="repo-list">
      {
        ReasonReact.array(Method.buildPublishRepoItem(state.publishRepoArray))
      }
    </div>
    {
      state.isProjectOver ?
        ReasonReact.null :
        state.publishRepoArray |> ArrayUtils.hasItem ?
          <div
            className="repo-load"
            onClick={_e => Method.loadHotPublishProject(state, send)}>
            {DomHelper.textEl({j|加载更多|j})}
          </div> :
          ReasonReact.null
    }
  </div>;

let make = (~store: AppStore.appState, ~dispatch, ~languageType, _children) => {
  ...component,
  initialState: () => {
    publishRepoArray: [||],
    page: 1,
    count: 32,
    isProjectOver: false,
  },
  reducer,
  render: self => render(store, dispatch, languageType, self),
  didMount: ({state, send}: ReasonReact.self('a, 'b, 'c)) =>
    Method.loadHotPublishProject(state, send),
};