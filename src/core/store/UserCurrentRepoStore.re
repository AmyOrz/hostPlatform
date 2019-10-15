type userRepoAction('a) =
  | ChangeRepo('a);

type userRepo = {
  id: int,
  userId: int,
  name: string,
  description: string,
  views: int,
  forks: int,
  createDate: string,
  access: int,
  size: float,
  filePath: string,
  artPicture: string,
};

type userRepoState = {userRepo: option(userRepo)};

let userRepoReducer = (state: userRepoState, action: userRepoAction('a)) =>
  switch (action) {
  | ChangeRepo(value) => {...state, userRepo: Some(value)}
  };