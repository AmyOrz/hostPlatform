external convertJsExnToArrayStr: Js.Exn.t => array(string) = "%identity";

let unsafeGetErrorMssage = exn =>
  switch (exn |> convertJsExnToArrayStr |> ArrayUtils.getLast) {
  | None => "execute sql param is undefined"
  | Some(error) => error
  };