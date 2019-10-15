open WonderBsMost;

open AppStore;

let getUserRepoPublishQuery = id => {j|
    {
      repoPublish(repoId:$id){
          id
          use_worker
      }
    }
|j};

let getUserRepoPublish = repoId =>
  Fetch.fetch(
    SystemConfig.getServerPath()
    ++ "/graphql?query="
    ++ getUserRepoPublishQuery(repoId),
  )
  |> Most.fromPromise
  |> Most.flatMap(response =>
       response |> Fetch.Response.json |> Most.fromPromise
     )
  |> Most.map(response => JsonType.convertToJsObj(response)##data);