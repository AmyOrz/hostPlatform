open Antd;

open UserCurrentRepoStore;

open WonderBsMost;

open WonderBsJson;

open AppStore;

type state = {
  repoName: string,
  repoDescription: string,
  access: int,
};

type action =
  | ChangeName(string)
  | ChangeDescription(string)
  | ChangeAccess(int);

module Method = {
  let imgUpload = (store, dispatch, repoId, event) => {
    let e = ReactEventType.convertReactFormEventToJsEvent(event);
    DomHelper.preventDefault(e);

    let chunkSize = SystemConfig.getFileChunkSize();
    let imgFile =
      e##target##files
      |> Js.Dict.values
      |> Js.Array.map(FileType.convertFileJsObjectToFile)
      |> ArrayUtils.unsafeGetFirst;
    let fileName: string = imgFile##name;
    let postfix =
      fileName
      |> Js.String.sliceToEnd(~from=fileName |> Js.String.lastIndexOf("."));

    switch (imgFile |> Image.getImageType) {
    | "image/jpeg"
    | "image/png" =>
      Most.just(imgFile)
      |> FileUploadUtils.getFileHashValue(fileName)
      |> Most.flatMap(fileHash => {
           let blockCount =
             Js.Math.ceil(
               (imgFile##size |> float_of_int) /. (chunkSize |> float_of_int),
             );

           (
             fileHash,
             blockCount,
             FileReader.buildFileChunkFromDataArray(
               (blockCount, chunkSize),
               fileHash,
               imgFile,
             ),
           )
           |> Most.just;
         })
      |> Most.flatMap(((fileHash, blockCount, fileChunkArray)) =>
           fileChunkArray
           |> Most.from
           |> Most.flatMap(formData =>
                Fetch.fetchWithInit(
                  SystemConfig.getServerPath() ++ "/uploadFileChunk",
                  Fetch.RequestInit.make(
                    ~method_=Post,
                    ~body=Fetch.BodyInit.makeWithFormData(formData),
                    (),
                  ),
                )
                |> Most.fromPromise
                |> MostHelper.ignore
              )
           |> Most.concat(
                MostHelper.callStreamFunc(() =>
                  Fetch.fetchWithInit(
                    SystemConfig.getServerPath() ++ "/uploadUserRepoArtPicture",
                    Fetch.RequestInit.make(
                      ~method_=Post,
                      ~body=
                        Fetch.BodyInit.make(
                          Js.Json.stringify(
                            Json.Encode.object_([
                              ("total", Json.Encode.int(blockCount)),
                              ("hash", Json.Encode.string(fileHash)),
                              (
                                "userName",
                                Json.Encode.string(
                                  StoreUtils.getUserName(store),
                                ),
                              ),
                              ("repoId", Json.Encode.int(repoId)),
                              ("postfix", Json.Encode.string(postfix)),
                            ]),
                          ),
                        ),
                      ~headers=
                        Fetch.HeadersInit.make({
                          "Content-Type": "application/json",
                        }),
                      (),
                    ),
                  )
                  |> Most.fromPromise
                  |> Most.flatMap(fetchResponse =>
                       fetchResponse |> Fetch.Response.json |> Most.fromPromise
                     )
                  |> Most.tap(result => {
                       let resultObj = result |> JsonType.convertToJsObj;

                       switch (
                         resultObj##status
                         |> UserRouteType.UploadUserRepoArtPictureType.convertIntToSelf
                       ) {
                       | Success =>
                         let userRepoArray =
                           store.userRepoArrayState.userRepoArray;

                         dispatch(
                           UserRepoArrayAction(
                             ChangeRepoArray(
                               userRepoArray
                               |> Js.Array.map(repoItem =>
                                    ValueUtils.isValueEqual(
                                      repoItem.id,
                                      repoId,
                                    ) ?
                                      {
                                        ...repoItem,
                                        artPicture: resultObj##data##imgPath,
                                      } :
                                      repoItem
                                  )
                               |> WonderLog.Log.print,
                             ),
                           ),
                         );

                       | ChunkCountError =>
                         Message.error(
                           {j|上传失败，图片片段数不匹配，请联系管理员|j},
                           10,
                         )
                       | InValidParam
                       | ParamError =>
                         Message.error({j|上传失败，参数错误|j}, 10)
                       | DBSqlError => Message.error(resultObj##msg, 10)
                       | UserRepoNotExist =>
                         Message.error(
                           {j|用户信息不存在，请联系管理员|j},
                           10,
                         )
                       };
                     })
                  |> MostHelper.ignore
                ),
              )
         )
      |> WonderBsMost.Most.drain
      |> ignore
    | _ =>
      Message.error({j|上传图片失败，请上传jpg/png图片|j}, 10)
    };
  };

  let submit = (store, dispatch, repoId, state) =>
    Fetch.fetchWithInit(
      SystemConfig.getServerPath() ++ "/updateUserRepoInfo",
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=
          Fetch.BodyInit.make(
            Js.Json.stringify(
              Json.Encode.object_([
                ("repoId", Json.Encode.string(repoId |> string_of_int)),
                ("name", Json.Encode.string(state.repoName)),
                ("description", Json.Encode.string(state.repoDescription)),
                ("access", Json.Encode.int(state.access)),
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

         switch (
           resultObj##status
           |> UserRouteType.UpdateUserRepoInfoType.convertIntToSelf
         ) {
         | Success =>
           UserRepoUtils.getAllUserRepo(
             StoreUtils.getUserId(store),
             dispatch,
           );
           Message.success({j|修改成功|j}, 5);

         | UserRepoNotExist =>
           Message.error({j|项目找不到, 请联系管理员|j}, 10)
         | ParamError
         | InValidParam => Message.error({j|参数错误|j}, 10)
         | DBSqlError =>
           let msg = resultObj##msg;
           Message.error({j|服务器错误: $msg|j}, 10);
         };
       })
    |> Most.drain
    |> ignore;
};

let component = ReasonReact.reducerComponent("ProjectSetting");

let reducer = (action, state) =>
  switch (action) {
  | ChangeName(value) => ReasonReact.Update({...state, repoName: value})
  | ChangeDescription(value) =>
    ReasonReact.Update({...state, repoDescription: value})
  | ChangeAccess(value) => ReasonReact.Update({...state, access: value})
  };

let render =
    (
      store,
      dispatch,
      currentRepo,
      languageType,
      {state, send}: ReasonReact.self('a, 'b, 'c),
    ) =>
  <div className="wonder-project-setting">
    <div className="user-item">
      <div className="item-title">
        {DomHelper.textEl({j|艺术插图|j})}
      </div>
      <div className="item-content">
        <UserImgUpload
          picturePath={SystemConfig.getServerPath() ++ currentRepo.artPicture}
          imgUploadFunc={Method.imgUpload(store, dispatch, currentRepo.id)}
        />
      </div>
    </div>
    <div className="user-item">
      <div className="item-title">
        {DomHelper.textEl({j|项目名称|j})}
      </div>
      <div className="item-content">
        <Input
          type_="text"
          size="large"
          value={state.repoName}
          onChange={
            event => send(ChangeName(ReactEventType.getFormValue(event)))
          }
        />
      </div>
    </div>
    <div className="user-item">
      <div className="item-title">
        {DomHelper.textEl({j|项目描述|j})}
      </div>
      <div className="item-content">
        <Input.TextArea
          value={state.repoDescription}
          autosize={"minRows": 2, "maxRows": 6}
          style={"marginTop": 15}
          onChange={
            event =>
              send(ChangeDescription(ReactEventType.getFormValue(event)))
          }
        />
      </div>
    </div>
    <div className="user-item">
      <div className="item-title">
        {DomHelper.textEl({j|项目权限|j})}
      </div>
      <div className="item-content">
        <Radio.Group value={state.access} style={"marginTop": 0}>
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
      </div>
    </div>
    <div className="user-item">
      <div
        className="item-save"
        onClick={_e => Method.submit(store, dispatch, currentRepo.id, state)}>
        <div className="save-text"> {DomHelper.textEl({j|保存|j})} </div>
        <div className="save-img"> <img src="/img/save.png" /> </div>
      </div>
    </div>
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
    repoName: currentRepo.name,
    repoDescription: currentRepo.description,
    access: currentRepo.access,
  },
  render: self => render(store, dispatch, currentRepo, languageType, self),
  reducer,
};