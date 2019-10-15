let getLocaleString = date =>
  MomentRe.Moment.toDate(date)
  |> Js.Date.toLocaleDateString
  |> WonderLog.Log.print;