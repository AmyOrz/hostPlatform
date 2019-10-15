module ActiveUserType = {
  type activeUserType =
    | Success
    | ParamError
    | DBSqlError
    | AlreadyActive
    | CodeError
    | UserNameNotExist;

  external convertSelfToInt: activeUserType => int = "%identity";

  external convertIntToSelf: int => activeUserType = "%identity";
};

module CheckUserLoginType = {
  type checkUserLoginType =
    | Success
    | ParamError
    | InValidParam
    | DBSqlError
    | UserNotActive
    | PasswordError
    | UserNameNotExist;

  external convertSelfToInt: checkUserLoginType => int = "%identity";

  external convertIntToSelf: int => checkUserLoginType = "%identity";
};

module GetRequestType = {
  type getRequestType =
    | Success
    | ParamError
    | DBSqlError;

  external convertSelfToInt: getRequestType => int = "%identity";

  external convertIntToSelf: int => getRequestType = "%identity";
};

module RegisterUserType = {
  type registerUserType =
    | Success
    | ParamError
    | DBSqlError
    | InValidParam
    | SendEmailError;

  external convertSelfToInt: registerUserType => int = "%identity";

  external convertIntToSelf: int => registerUserType = "%identity";
};

module SendEmailToRetrievePasswordType = {
  type sendEmailType =
    | Success
    | Fail
    | ParamError
    | DBSqlError
    | UserNameNotExist;

  external convertSelfToInt: sendEmailType => int = "%identity";
};

module UploadUserProfileType = {
  type uploadUserProfileType =
    | Success
    | InValidParam
    | ParamError
    | DBSqlError
    | ChunkCountError
    | UserInfoNotExist;

  external convertSelfToInt: uploadUserProfileType => int = "%identity";

  external convertIntToSelf: int => uploadUserProfileType = "%identity";
};

module UploadFileChunkType = {
  type uploadFileChunkType =
    | Success
    | OperateFileError
    | ParamError;

  external convertSelfToInt: uploadFileChunkType => int = "%identity";

  external convertIntToSelf: int => uploadFileChunkType = "%identity";
};

module AddUserRepoType = {
  type addUserRepoType =
    | Success
    | InValidParam
    | DBSqlError
    | ParamError;

  external convertSelfToInt: addUserRepoType => int = "%identity";

  external convertIntToSelf: int => addUserRepoType = "%identity";
};

module UploadUserRepoFileType = {
  type uploadUserRepoFileType =
    | Success
    | InValidParam
    | DBSqlError
    | UserRepoNotExist
    | UserIdNotRelationRepoId
    | ChunkCountError
    | ParamError;

  external convertSelfToInt: uploadUserRepoFileType => int = "%identity";

  external convertIntToSelf: int => uploadUserRepoFileType = "%identity";
};

module UploadUserRepoArtPictureType = {
  type uploadUserRepoArtPictureType =
    | Success
    | InValidParam
    | DBSqlError
    | UserRepoNotExist
    | ChunkCountError
    | ParamError;

  external convertSelfToInt: uploadUserRepoArtPictureType => int = "%identity";

  external convertIntToSelf: int => uploadUserRepoArtPictureType = "%identity";
};

module DeleteUserRepoType = {
  type deleteUserRepoType =
    | Success
    | InValidParam
    | DBSqlError
    | UserRepoNotExist
    | UserIdNotRelationRepoId
    | ParamError;

  external convertSelfToInt: deleteUserRepoType => int = "%identity";

  external convertIntToSelf: int => deleteUserRepoType = "%identity";
};

module UpdateUserRepoInfoType = {
  type updateUserRepoInfoType =
    | Success
    | InValidParam
    | DBSqlError
    | UserRepoNotExist
    | ParamError;

  external convertSelfToInt: updateUserRepoInfoType => int = "%identity";

  external convertIntToSelf: int => updateUserRepoInfoType = "%identity";
};

module RepoPublishType = {
  type repoPublishType =
    | Success
    | ChunkCountError
    | InValidParam
    | DBSqlError
    | ParamError;

  external convertSelfToInt: repoPublishType => int = "%identity";

  external convertIntToSelf: int => repoPublishType = "%identity";
};

module GetAssetBundleDirPathType = {
  type getAssetBundleDirPathType =
    | Success
    | InValidParam
    | ParamError;

  external convertSelfToInt: getAssetBundleDirPathType => int = "%identity";

  external convertIntToSelf: int => getAssetBundleDirPathType = "%identity";
};