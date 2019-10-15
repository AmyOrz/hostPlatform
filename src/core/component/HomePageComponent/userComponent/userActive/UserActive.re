open JustgageReasonCookie;

open WonderBsMost;

open Antd;

type state = {logMsg: string};

type action =
  | ChangeLogMsg(string);

module Method = {
  let userSign = () => {};
};

let component = ReasonReact.reducerComponent("UserActive");

let reducer = (action, state) =>
  switch (action) {
  | ChangeLogMsg(value) => ReasonReact.Update({...state, logMsg: value})
  };

let make = _children => {
  ...component,
  initialState: () => {logMsg: "wef"},
  reducer,
  render: ({state}: ReasonReact.self('a, 'b, 'c)) =>
    <div className="wonder-userActive">
      <Modal
        visible=true
        title={j|用户激活|j}
        footer=[|
          <Button
            key="submit"
            type_="primary"
            size="large"
            onClick={_e => DomHelper.locationHref("/sign")}>
            {DomHelper.textEl({j|确定|j})}
          </Button>,
        |]>
        <div className=""> {DomHelper.textEl(state.logMsg)} </div>
      </Modal>
    </div>,
  didMount: ({send}: ReasonReact.self('a, 'b, 'c)) => {
    let param = DomHelper.locationSearch(.);
    let userName = param##userName;
    let code = param##code;

    switch (
      userName |> ValueUtils.isValueValid,
      code |> ValueUtils.isValueValid,
    ) {
    | (true, true) =>
      Fetch.fetch(
        SystemConfig.getServerPath()
        ++ {j|/activeUser?userName=$userName&code=$code|j},
      )
      |> Most.fromPromise
      |> Most.flatMap(response =>
           response |> Fetch.Response.json |> Most.fromPromise
         )
      |> Most.tap(result => {
           let resultObj = JsonType.convertToJsObj(result);

           switch (
             resultObj##status |> UserRouteType.ActiveUserType.convertIntToSelf
           ) {
           | Success =>
             send(
               ChangeLogMsg({j|激活成功，请点击确定进行登录|j}),
             )
           | ParamError => send(ChangeLogMsg({j|参数错误|j}))
           | DBSqlError => send(ChangeLogMsg(resultObj##error))
           | AlreadyActive =>
             send(ChangeLogMsg({j|用户已经激活，请直接登录|j}))
           | CodeError => send(ChangeLogMsg({j|激活码错误|j}))
           | UserNameNotExist => send(ChangeLogMsg({j|用户名错误|j}))
           };
         })
      |> WonderBsMost.Most.drain
      |> ignore
    | _ => send(ChangeLogMsg("参数错误，请重新发送邮件"))
    };
  },
};