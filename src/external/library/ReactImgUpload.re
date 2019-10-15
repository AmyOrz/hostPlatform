module Upload = {
  [@bs.module "react-core-image-upload/src/index"]
  external reactClass : ReasonReact.reactClass = "default";
  let make =
      (
        ~url: option(string)=?,
        ~text: option(string)=?,
        ~inputOfFile: option(string)=?,
        ~maxFileSize: option(int)=?,
        ~imageUploaded: option('value => unit)=?,
        children,
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass,
      ~props={
        "url": Js.Nullable.fromOption(url),
        "text": Js.Nullable.fromOption(text),
        "inputOfFile": Js.Nullable.fromOption(inputOfFile),
        "maxFileSize": Js.Nullable.fromOption(maxFileSize),
        "imageUploaded": Js.Nullable.fromOption(imageUploaded),
      },
      children,
    );
};