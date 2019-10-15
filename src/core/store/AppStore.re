open UserInfoStore;

open UserRepoArrayStore;

open UserCurrentRepoStore;

type appState = {
  isGetUserData: bool,
  userUsedSize: float,
  userInfoState,
  userRepoState,
  userRepoArrayState,
};

type ReduxThunk.thunk('a) +=
  | ReplaceState('a);

type ReduxThunk.thunk(_) +=
  | GetUserData
  | ChangeUsedSize(float)
  | UserInfoAction(userInfoAction(userInfo))
  | UserRepoAction(userRepoAction(userRepo))
  | UserRepoArrayAction(userRepoArrayAction(array(userRepo)));

let state: appState = {
  isGetUserData: false,
  userUsedSize: 0.,
  userInfoState: {
    userInfo: None,
  },
  userRepoState: {
    userRepo: None,
  },
  userRepoArrayState: {
    userRepoArray: [||],
  },
};

let appReducter = (state: appState, action) =>
  switch (action) {
  | GetUserData => {...state, isGetUserData: true}
  | ChangeUsedSize(value) => {...state, userUsedSize: value}
  | UserInfoAction(action) => {
      ...state,
      userInfoState: userInfoReducer(state.userInfoState, action),
    }
  | UserRepoAction(action) => {
      ...state,
      userRepoState: userRepoReducer(state.userRepoState, action),
    }
  | UserRepoArrayAction(action) => {
      ...state,
      userRepoArrayState:
        userRepoArrayReducer(state.userRepoArrayState, action),
    }
  | ReplaceState(replacedState) => replacedState
  | _ => state
  };