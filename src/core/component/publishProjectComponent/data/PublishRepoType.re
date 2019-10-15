type publishRepo = {
  repoId: int,
  repoName: string,
  views: int,
  artPicture: string,
  userName: string,
  userId: int,
  useWorker: int,
};

type state = {
  page: int,
  count: int,
  isProjectOver: bool,
  publishRepoArray: array(publishRepo),
};

type action =
  | ChangePage(int)
  | ChangeIsProjectOver
  | ChangePublishRepo(array(publishRepo));
