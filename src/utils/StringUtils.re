let sliceHalfString = string => {
  let index = (string |> Js.String.length) / 2;

  string |> Js.String.slice(~from=0, ~to_=index);
};

let isValueValid = str => str !== "" && ValueUtils.isValueValid(str);