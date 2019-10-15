open WonderBsMost;

open JustgageReasonCookie;

open WonderBsJson;

open Antd;

type state = {
  userName: string,
  password: string,
  logMsg: string,
};

type action =
  | ChangeUserName(string)
  | ChangePassword(string)
  | ChangeLogMsg(string);

module Method = {
  let submit = (userName, password, send) =>
    Fetch.fetchWithInit(
      SystemConfig.getServerPath() ++ "/checkUserLogin",
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=
          Fetch.BodyInit.make(
            Js.Json.stringify(
              Json.Encode.object_([
                ("userName", Json.Encode.string(userName)),
                ("password", Json.Encode.string(password)),
              ]),
            ),
          ),
        ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
        (),
      ),
    )
    |> Most.fromPromise
    |> Most.flatMap(response =>
         response |> Fetch.Response.json |> Most.fromPromise
       )
    |> Most.tap(result => {
         let resultObj = JsonType.convertToJsObj(result);

         switch (
           resultObj##status
           |> UserRouteType.CheckUserLoginType.convertIntToSelf
         ) {
         | Success =>
           let responseData = resultObj##data;
           Cookie.setJson(
             "userData",
             Json.Encode.object_([
               ("userId", Json.Encode.int(responseData##id)),
               ("userName", Json.Encode.string(responseData##userName)),
             ]),
           );

           DomHelper.locationHref("/");

         | UserNotActive =>
           send(
             ChangeLogMsg(
               {j|用户未激活，我们已经发送邮件到您的邮箱，请到邮箱进行激活。|j},
             ),
           )
         | ParamError
         | InValidParam => send(ChangeLogMsg({j|参数错误|j}))
         | DBSqlError => send(ChangeLogMsg(resultObj##msg))
         | PasswordError
         | UserNameNotExist =>
           send(
             ChangeLogMsg({j|用户名或密码错误，请重新登录|j}),
           )
         };
       })
    |> Most.drain
    |> ignore;
  /* let retrievePassword = (userName, send) =>
     Fetch.fetch({j|/sendEmailToRetrievePassword?userName=$userName|j})
     |> WonderBsMost.Most.fromPromise
     |> WonderBsMost.Most.flatMap(response =>
          response
          |> Fetch.Response.json
          |> WonderBsMost.Most.fromPromise
          |> WonderBsMost.Most.map(response => {
               let resultObj = JsonType.convertToJsObj(response);

               resultObj##status === 1 ?
                 {
                   let email = resultObj##email;

                   send(
                     ChangeLogMsg(
                       {j|我们已经发送邮件到$email， 请到邮箱进行验证.|j},
                     ),
                   );
                 } :
                 send(ChangeLogMsg(resultObj##error));
             })
        )
     |> Most.drain
     |> ignore; */
};

let component = ReasonReact.reducerComponent("UserSignIn");

let reducer = (action, state) =>
  switch (action) {
  | ChangeUserName(value) => ReasonReact.Update({...state, userName: value})
  | ChangePassword(value) => ReasonReact.Update({...state, password: value})
  | ChangeLogMsg(value) => ReasonReact.Update({...state, logMsg: value})
  };

let render = ({state, send}: ReasonReact.self('a, 'b, 'c)) =>
  <div className="wonder-signIn">
    {
      state.logMsg |> StringUtils.isValueValid ?
        <Alert message={state.logMsg} type_="error" showIcon=true /> :
        ReasonReact.null
    }
    <Input
      type_="text"
      size="large"
      value={state.userName}
      placeholder="Username"
      onChange={
        event => send(ChangeUserName(ReactEventType.getFormValue(event)))
      }
    />
    <Input
      type_="password"
      size="large"
      value={state.password}
      placeholder="Password"
      onChange={
        event => send(ChangePassword(ReactEventType.getFormValue(event)))
      }
    />
    <Button
      className="submit-btn"
      disabled={state.userName === "" || state.password === ""}
      onClick={_ => Method.submit(state.userName, state.password, send)}>
      {DomHelper.textEl("Sign In")}
    </Button>
  </div>;

let make = _children => {
  ...component,
  render,
  initialState: () => {userName: "", password: "", logMsg: ""},
  reducer,
};