let convertToStr = jsonResult =>
  jsonResult
  |> Js.Json.stringify
  |> Js.String.replaceByRe([%re {|/\"/g|}], "");