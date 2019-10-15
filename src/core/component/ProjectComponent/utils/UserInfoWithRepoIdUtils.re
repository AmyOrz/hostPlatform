open WonderBsMost;

open AppStore;

let getUserInfoWithRepoIdQuery = (userId, repoId) => {j|
    {
      loginUser(id:$userId){
        user_name
        email
        create_date
      }
      userInfo(userId:$userId){
        profile_picture
        nick_name
        hash_code
      }
      userRepo(id:$repoId){
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

let getUserInfoDataWithRepoId = (userId, repoId, dispatch) =>
  Fetch.fetch(
    SystemConfig.getServerPath()
    ++ "/graphql?query="
    ++ getUserInfoWithRepoIdQuery(userId, repoId),
  )
  |> Most.fromPromise
  |> Most.flatMap(response =>
       response |> Fetch.Response.json |> Most.fromPromise
     )
  |> Most.tap(response => {
       let resultObj = JsonType.convertToJsObj(response)##data;

       switch (
         resultObj##loginUser |> ArrayUtils.getFirst,
         resultObj##userInfo |> ArrayUtils.getFirst,
         resultObj##userRepo |> ArrayUtils.getFirst,
       ) {
       | (Some(userLogin), Some(userInfo), Some(userRepo)) =>
         dispatch(
           UserInfoAction(
             ChangeInfo({
               userId,
               userName: userLogin##user_name,
               email: userLogin##email,
               createDate: userLogin##create_date,
               profilePath: userInfo##profile_picture,
               nickName: userInfo##nick_name,
               hashCode: userInfo##hash_code,
             }),
           ),
         );
         dispatch(
           UserRepoAction(
             ChangeRepo(userRepo |> UserRepoUtils.convertJsObjToRecord),
           ),
         );
         dispatch(GetUserData);

       | _ =>
         JustgageReasonCookie.Cookie.remove("userData");

         DomHelper.locationHref("/");
       };
     });