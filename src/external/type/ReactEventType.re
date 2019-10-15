external convertReactMouseEventToJsEvent: ReactEventRe.Mouse.t => Js.t({..}) =
  "%identity";

external convertReactFormEventToJsEvent: ReactEventRe.Form.t => Js.t({..}) =
  "%identity";

let getFormValue = event =>
  ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value;