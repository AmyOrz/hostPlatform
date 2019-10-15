open Antd;

open WonderBsMost;

open WonderBsJson;

open UserCurrentRepoStore;

open AppStore;

type state = {
  repoName: string,
  repoDescription: string,
  repoAccess: int,
  isShowCreateProjectModal: bool,
};

type action =
  | ChangeRepoName(string)
  | ChangeRepoDescription(string)
  | ShowCreateProjectModal
  | HideCreateProjectModal;

module Method = {
  let handleOk = (store, dispatch, state, send) => {
    let userId = StoreUtils.getUserId(store);
    let userName = StoreUtils.getUserName(store);

    Fetch.fetchWithInit(
      SystemConfig.getServerPath() ++ "/addUserRepo",
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=
          Fetch.BodyInit.make(
            Js.Json.stringify(
              Json.Encode.object_([
                ("userId", Json.Encode.int(userId)),
                ("userName", Json.Encode.string(userName)),
                ("name", Json.Encode.string(state.repoName)),
                ("description", Json.Encode.string(state.repoDescription)),
                ("access", Json.Encode.int(state.repoAccess)),
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
    |> Most.tap(response => {
         let resultObj = JsonType.convertToJsObj(response);

         switch (
           resultObj##status |> UserRouteType.AddUserRepoType.convertIntToSelf
         ) {
         | Success =>
           UserRepoUtils.getAllUserRepo(userId, dispatch);
           send(HideCreateProjectModal);
           Message.success({j|创建成功|j}, 5);

         | InValidParam
         | DBSqlError
         | ParamError =>
           Message.error({j|添加失败，请联系管理员|j}, 10)
         };
       })
    |> Most.drain
    |> ignore;
  };

  let handleCancel = send => send(HideCreateProjectModal);

  let removeRepo = (userId, userName, repoId, dispatch) =>
    Fetch.fetchWithInit(
      SystemConfig.getServerPath() ++ "/deleteUserRepo",
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=
          Fetch.BodyInit.make(
            Js.Json.stringify(
              Json.Encode.object_([
                ("userName", Json.Encode.string(userName)),
                ("userId", Json.Encode.int(userId)),
                ("repoId", Json.Encode.int(repoId)),
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
    |> Most.tap(response => {
         let resultObj = JsonType.convertToJsObj(response);

         switch (
           resultObj##status
           |> UserRouteType.DeleteUserRepoType.convertIntToSelf
         ) {
         | Success =>
           UserRepoUtils.getAllUserRepo(userId, dispatch);
           Message.success({j|删除成功|j}, 5);

         | InValidParam
         | DBSqlError
         | ParamError =>
           Message.error({j|添加失败，请联系管理员|j}, 10)

         | UserRepoNotExist
         | UserIdNotRelationRepoId =>
           Message.error({j|项目与用户不匹配|j}, 10)
         };
       })
    |> Most.drain
    |> ignore;

  let buildAllRepoComponent = (store, dispatch) => {
    let userId = StoreUtils.getUserId(store);

    StoreUtils.getUserRepoArray(store)
    |> Js.Array.map(
         ({id, name, views, forks, createDate, access, artPicture}) =>
         <div className="repo-item" key={id |> string_of_int}>
           <div
             className="item-profile"
             onClick={
               _e => DomHelper.locationHref({j|targetProject?repoId=$id|j})
             }>
             <img src={SystemConfig.getServerPath() ++ artPicture} />
           </div>
           <div className="item-content">
             <div
               className="item-name"
               onClick={
                 _e => DomHelper.locationHref({j|targetProject?repoId=$id|j})
               }>
               {DomHelper.textEl({j|$name|j})}
             </div>
             <div className="item-data">
               <div className="data-item">
                 <div className="item-title title-date">
                   {DomHelper.textEl({j|创建时间： |j})}
                 </div>
                 <div className="item-date">
                   {DomHelper.textEl(createDate)}
                 </div>
               </div>
               <div className="data-item">
                 <div className="item-title title-access">
                   {DomHelper.textEl({j|权限： |j})}
                 </div>
                 <div className="item-access">
                   {DomHelper.textEl({j|公有项目|j})}
                 </div>
               </div>
               <div className="data-item">
                 <div className="item-title">
                   <img src="/img/view.png" />
                 </div>
                 <div className="item-number">
                   {DomHelper.textEl(views |> string_of_int)}
                 </div>
               </div>
               <div className="data-item">
                 <div className="item-title">
                   <img src="/img/fork.png" />
                 </div>
                 <div className="item-number">
                   {DomHelper.textEl(forks |> string_of_int)}
                 </div>
               </div>
             </div>
             <div className="item-operate">
               <div className="operate-item">
                 <Button size="small" icon="export">
                   {DomHelper.textEl({j|导出项目|j})}
                 </Button>
               </div>
               <div className="operate-item">
                 <Button
                   size="small"
                   icon="delete"
                   type_="danger"
                   onClick={
                     _e =>
                       removeRepo(
                         userId,
                         StoreUtils.getUserName(store),
                         id,
                         dispatch,
                       )
                   }>
                   {DomHelper.textEl({j|删除|j})}
                 </Button>
               </div>
             </div>
           </div>
         </div>
       );
  };

  let buildAddRepoModalComponent = (store, dispatch, state, send) =>
    <Modal
      visible=true
      title={j|添加项目|j}
      onOk={
        _e =>
          StringUtils.isValueValid(state.repoName) ?
            handleOk(store, dispatch, state, send) :
            Antd.Message.error({j|项目名称不能为空|j}, 4)
      }
      onCancel={_e => handleCancel(send)}>
      <Input
        type_="text"
        size="large"
        placeholder={j|项目名称|j}
        value={state.repoName}
        onChange={
          event => send(ChangeRepoName(ReactEventType.getFormValue(event)))
        }
      />
      <Input.TextArea
        value={state.repoDescription}
        placeholder={j|项目描述|j}
        autosize={"minRows": 2, "maxRows": 6}
        style={"marginTop": 15}
        onChange={
          event =>
            send(ChangeRepoDescription(ReactEventType.getFormValue(event)))
        }
      />
      <Radio.Group value={state.repoAccess} style={"marginTop": 15}>
        <Radio.Button value=0>
          {DomHelper.textEl({j|公有项目|j})}
        </Radio.Button>
        <Radio.Button value=1 disabled=true>
          {DomHelper.textEl({j|私有项目|j})}
        </Radio.Button>
      </Radio.Group>
      <span
        style={
          ReactDOMRe.Style.make(
            ~marginLeft="20px",
            ~color="rgba(50,68,71,0.5)",
            ~fontSize="13px",
            (),
          )
        }>
        {DomHelper.textEl({j|只有付费用户可以创建私有项目|j})}
      </span>
    </Modal>;
};

let component = ReasonReact.reducerComponent("UserProjectComponent");

let make = (~store: AppStore.appState, ~dispatch, ~languageType, _children) => {
  ...component,
  initialState: () => {
    repoName: "",
    repoDescription: "",
    repoAccess: 0,
    isShowCreateProjectModal: false,
  },
  reducer: (action, state) =>
    switch (action) {
    | ChangeRepoName(value) =>
      ReasonReact.Update({...state, repoName: value})
    | ChangeRepoDescription(value) =>
      ReasonReact.Update({...state, repoDescription: value})
    | ShowCreateProjectModal =>
      ReasonReact.Update({...state, isShowCreateProjectModal: true})
    | HideCreateProjectModal =>
      ReasonReact.Update({...state, isShowCreateProjectModal: false})
    },
  render: ({state, send}: ReasonReact.self('a, 'b, 'c)) =>
    <div className="wonder-user-project">
      <div className="project-nav">
        <div className="nav-item">
          <Button
            size="large"
            icon="plus"
            onClick={_e => send(ShowCreateProjectModal)}>
            {DomHelper.textEl({j|创建项目|j})}
          </Button>
        </div>
        <div className="nav-item">
          <div className="item-title item-active">
            {DomHelper.textEl({j|全部项目|j})}
          </div>
          <div className="item-title">
            {DomHelper.textEl({j|私有项目|j})}
          </div>
        </div>
        <div className="nav-item">
          <div className="item-upgrade">
            <div className="upgrade-title">
              {DomHelper.textEl({j|空间使用情况|j})}
            </div>
            <div className="upgrade-content">
              {
                DomHelper.textEl(
                  (StoreUtils.getUserUsedSize(store) |> string_of_float)
                  ++ " MB / 200 MB",
                )
              }
            </div>
            <div className="upgrade-title">
              {DomHelper.textEl({j|获取更多空间|j})}
            </div>
            <div className="upgrade-content">
              <Button size="large" icon="cloud-upload">
                {DomHelper.textEl({j|项目升级|j})}
              </Button>
            </div>
          </div>
        </div>
      </div>
      <div className="project-content">
        {ReasonReact.array(Method.buildAllRepoComponent(store, dispatch))}
      </div>
      {
        state.isShowCreateProjectModal ?
          Method.buildAddRepoModalComponent(store, dispatch, state, send) :
          ReasonReact.null
      }
    </div>,
};