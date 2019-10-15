type userInfoAction('a) =
  | ChangeInfo('a);

type userInfo = {
  userId: int,
  userName: string,
  email: string,
  createDate: string,
  profilePath: string,
  nickName: string,
  hashCode: string,
};

type userInfoState = {userInfo: option(userInfo)};

let userInfoReducer = (state: userInfoState, action: userInfoAction('a)) =>
  switch (action) {
  | ChangeInfo(value) => {...state, userInfo: Some(value)}
  };