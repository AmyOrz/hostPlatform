open WonderBsMost;

open AppStore;

open UserCurrentRepoStore;

let calcAllRepoUseSize = userRepos =>
  userRepos
  |> Js.Array.reduce((size, repoItem) => size +. repoItem##size, 0.)
  |> FloatUtils.truncateFloatValue(_, 2);

let convertJsObjToRecord = repoItem => {
  id: repoItem##id |> int_of_string,
  userId: repoItem##user_id,
  name: repoItem##name,
  description: repoItem##description,
  views: repoItem##views,
  forks: repoItem##forks,
  createDate: repoItem##create_date,
  access: repoItem##access,
  size: repoItem##size,
  filePath: repoItem##file_path,
  artPicture: repoItem##art_picture,
};

let getUserRepoRecord = userRepos =>
  userRepos |> Js.Array.map(repoItem => convertJsObjToRecord(repoItem));

let getUserReposQuery = userId => {j|
    {
      userRepos(userId:$userId){
          id
          user_id
          name
          description
          views
          forks
          create_date
          access
          size
          file_path
          art_picture
      }
    }
|j};

let getAllUserRepo = (userId, dispatch) =>
  Fetch.fetch(
    SystemConfig.getServerPath()
    ++ "/graphql?query="
    ++ getUserReposQuery(userId),
  )
  |> Most.fromPromise
  |> Most.flatMap(response =>
       response |> Fetch.Response.json |> Most.fromPromise
     )
  |> Most.tap(response => {
       let resultObj = JsonType.convertToJsObj(response)##data;

       dispatch(
         UserRepoArrayAction(
           ChangeRepoArray(getUserRepoRecord(resultObj##userRepos)),
         ),
       );

       dispatch(ChangeUsedSize(resultObj##userRepos |> calcAllRepoUseSize));
     })
  |> Most.drain
  |> ignore;