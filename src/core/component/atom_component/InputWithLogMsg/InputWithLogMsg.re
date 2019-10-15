open Antd;

let component = ReasonReact.statelessComponent("InputWithLogMsg");

let make =
    (
      ~value,
      ~onChange: string => unit,
      ~isSuccess: bool,
      ~successMsg: string,
      ~failMsg: string,
      ~id: option(string)=?,
      ~type_: option(string)=?,
      ~size: option(string)=?,
      ~label: option(string)=?,
      _children,
    ) => {
  ...component,
  render: _self =>
    <article className="wonder-input-logMsg" key="inputWithLogMsg">
      <Input
        id={
          switch (id) {
          | None => ""
          | Some(id) => id
          }
        }
        type_={
          switch (type_) {
          | None => "text"
          | Some(type_) => type_
          }
        }
        size={
          switch (size) {
          | None => "large"
          | Some(size) => size
          }
        }
        value
        placeholder={
          switch (label) {
          | None => ""
          | Some(label) => label
          }
        }
        onChange={event => onChange(ReactEventType.getFormValue(event))}
      />
      {
        isSuccess ?
          <Alert message=successMsg type_="success" showIcon=true /> :
          <Alert message=failMsg type_="error" showIcon=true />
      }
    </article>,
};