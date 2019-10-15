open Express;

type inComingForm;

[@bs.module "formidable"] [@bs.new]
external createIncomingForm : unit => inComingForm = "IncomingForm";

[@bs.send]
external parse :
  (inComingForm, Request.t, (string, Js.t({..}), Js.t({..})) => unit) => unit =
  "parse";