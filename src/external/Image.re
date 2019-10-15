let getImageNameFromPath = imgPath => {
  let imgStartIndex = imgPath |> Js.String.lastIndexOf("/");
  let imgPostfixIndex = imgPath |> Js.String.lastIndexOf("?");

  ValueUtils.isValueEqual(imgPostfixIndex, -1) ?
    imgPath |> Js.String.sliceToEnd(~from=imgStartIndex + 1) :
    imgPath
    |> Js.String.slice(
         ~from=(imgPath |> Js.String.lastIndexOf("/")) + 1,
         ~to_=imgPostfixIndex,
       );
};

let getImageType = [%raw image => {|return image.type|}];