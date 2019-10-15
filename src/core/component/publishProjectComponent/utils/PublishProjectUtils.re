open WonderBsMost;

open PublishRepoType;

let convertToPublishRepoRecord = publishRepoArray =>
  publishRepoArray
  |> Js.Array.map(item =>
       {
         repoId: item##repo_id,
         repoName: item##name,
         views: item##views,
         artPicture: item##art_picture,
         userName: item##user_name,
         userId: item##user_id,
         useWorker: item##use_worker,
       }
     );

let getHotPublishProject = (page, count, publishRepoArray, send) =>
  Fetch.fetch(
    SystemConfig.getServerPath()
    ++ {j|/getHotPublishRepo?page=$page&count=$count|j},
  )
  |> Most.fromPromise
  |> Most.flatMap(response =>
       response |> Fetch.Response.json |> Most.fromPromise
     )
  |> Most.tap(response => {
       let resultObj = JsonType.convertToJsObj(response);

       switch (
         resultObj##status |> UserRouteType.GetRequestType.convertIntToSelf
       ) {
       | Success =>
         let resultRepoArray = resultObj##data##hotPublishRepo;

         publishRepoArray |> Js.Array.length < count ?
           send(ChangeIsProjectOver) : send(ChangePage(page + 1));

         send(
           ChangePublishRepo(
             publishRepoArray
             |> Js.Array.concat(resultRepoArray |> convertToPublishRepoRecord),
           ),
         );

       | ParamError => Antd.Message.error({j|参数错误|j}, 6)

       | DBSqlError =>
         Antd.Message.error({j|数据库错误， 请联系管理员|j}, 6)
       };
     });

let getNewPublishProject = (page, count, publishRepoArray, send) =>
  Fetch.fetch(
    SystemConfig.getServerPath()
    ++ {j|/getNewPublishRepo?page=$page&count=$count|j},
  )
  |> Most.fromPromise
  |> Most.flatMap(response =>
       response |> Fetch.Response.json |> Most.fromPromise
     )
  |> Most.tap(response => {
       let resultObj = JsonType.convertToJsObj(response);

       switch (
         resultObj##status |> UserRouteType.GetRequestType.convertIntToSelf
       ) {
       | Success =>
         let resultRepoArray = resultObj##data##newPublishRepo;

         publishRepoArray |> Js.Array.length < count ?
           send(ChangeIsProjectOver) : send(ChangePage(page + 1));

         send(
           ChangePublishRepo(
             publishRepoArray
             |> Js.Array.concat(resultRepoArray |> convertToPublishRepoRecord),
           ),
         );

       | ParamError => Antd.Message.error({j|参数错误|j}, 6)

       | DBSqlError =>
         Antd.Message.error({j|数据库错误， 请联系管理员|j}, 6)
       };
     });