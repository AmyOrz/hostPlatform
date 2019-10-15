open WonderBsMost;

open AppStore;

let getUserInfoDataQuery = id => {j|
    {
      loginUser(id:$id){
        user_name
        email
        create_date
      }
      userInfo(userId:$id){
        profile_picture
        nick_name
        hash_code
      }
      userRepos(userId:$id){
          id
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

let getUserInfoData = (userId, dispatch) =>
  Fetch.fetch(
    SystemConfig.getServerPath()
    ++ "/graphql?query="
    ++ getUserInfoDataQuery(userId),
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
       ) {
       | (Some(userLogin), Some(userInfo)) =>
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
           UserRepoArrayAction(
             ChangeRepoArray(
               resultObj##userRepos |> UserRepoUtils.getUserRepoRecord,
             ),
           ),
         );
         dispatch(
           ChangeUsedSize(
             resultObj##userRepos |> UserRepoUtils.calcAllRepoUseSize,
           ),
         );
         dispatch(GetUserData);

       | _ =>
         JustgageReasonCookie.Cookie.remove("userData");

         DomHelper.locationHref("/");
       };
     });