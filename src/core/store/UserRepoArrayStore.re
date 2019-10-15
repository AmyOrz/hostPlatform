open UserCurrentRepoStore;

type userRepoArrayAction('a) =
  | ChangeRepoArray('a);

type userRepoArrayState = {userRepoArray: array(userRepo)};

let userRepoArrayReducer =
    (state: userRepoArrayState, action: userRepoArrayAction('a)) =>
  switch (action) {
  | ChangeRepoArray(value) => {...state, userRepoArray: value}
  };