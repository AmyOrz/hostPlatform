open Antd;

open WonderBsMost;

open WonderBsJson;

open AppStore;

open UserInfoStore;

module Method = {
  let imgUpload = (store, dispatch, event) => {
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
                    SystemConfig.getServerPath() ++ "/uploadUserProfile",
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
                              (
                                "userId",
                                Json.Encode.int(StoreUtils.getUserId(store)),
                              ),
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
                         |> UserRouteType.UploadUserProfileType.convertIntToSelf
                       ) {
                       | Success =>
                         let userInfoRecord =
                           store.userInfoState.userInfo |> Js.Option.getExn;

                         dispatch(
                           UserInfoAction(
                             ChangeInfo({
                               ...userInfoRecord,
                               profilePath: resultObj##data##imgPath,
                             }),
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
                       | UserInfoNotExist =>
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
};

let component = ReasonReact.statelessComponent("UserSettingComponent");

let render = (store, dispatch, languageType, _self) =>
  <div className="wonder-user-setting">
    <div className="user-item">
      <div className="item-title"> {DomHelper.textEl({j|头像|j})} </div>
      <div className="item-content">
        <UserImgUpload
          picturePath={
            SystemConfig.getServerPath()
            ++ StoreUtils.getUserProfilePath(store)
          }
          imgUploadFunc={Method.imgUpload(store, dispatch)}
        />
      </div>
    </div>
    <div className="user-item">
      <div className="item-title"> {DomHelper.textEl({j|用户名|j})} </div>
      <div className="item-content">
        <Input
          type_="text"
          size="large"
          value={StoreUtils.getUserName(store)}
          disabled=true
        />
      </div>
    </div>
    <div className="user-item">
      <div className="item-title">
        {DomHelper.textEl({j|用户昵称|j})}
      </div>
      <div className="item-content">
        <Input
          type_="text"
          size="large"
          value={StoreUtils.getUserNickName(store)}
          onChange={_e => Js.log(1)}
        />
      </div>
    </div>
    <div className="user-item">
      <div className="item-title"> {DomHelper.textEl({j|邮箱|j})} </div>
      <div className="item-content">
        <Input
          type_="text"
          size="large"
          value={StoreUtils.getUserEmail(store)}
          disabled=true
        />
      </div>
    </div>
  </div>;

let make = (~store: AppStore.appState, ~dispatch, ~languageType, _children) => {
  ...component,
  render: self => render(store, dispatch, languageType, self),
};