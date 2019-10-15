open UserInfoStore;

open UserRepoArrayStore;

open UserCurrentRepoStore;

let unsafeGetUserUnfo = (store: AppStore.appState) =>
  store.userInfoState.userInfo |> Js.Option.getExn;

let getUserId = (store: AppStore.appState) =>
  unsafeGetUserUnfo(store).userId;

let getUserName = (store: AppStore.appState) =>
  unsafeGetUserUnfo(store).userName;

let getUserEmail = (store: AppStore.appState) =>
  unsafeGetUserUnfo(store).email;

let getUserCreateDate = (store: AppStore.appState) =>
  unsafeGetUserUnfo(store).createDate;

let getUserProfilePath = (store: AppStore.appState) =>
  unsafeGetUserUnfo(store).profilePath;

let getUserNickName = (store: AppStore.appState) =>
  unsafeGetUserUnfo(store).nickName;

let getUserHashCode = (store: AppStore.appState) =>
  unsafeGetUserUnfo(store).hashCode;

let getUserRepoArray = (store: AppStore.appState) =>
  store.userRepoArrayState.userRepoArray;

let getUserUsedSize = (store: AppStore.appState) => store.userUsedSize;

let unsafeGetCurrentRepo = (store: AppStore.appState) =>
  store.userRepoState.userRepo |> Js.Option.getExn;