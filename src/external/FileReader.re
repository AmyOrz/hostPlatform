open FileType;

type fileReader;

type resultType;

[@bs.new] external createFileReader: unit => fileReader = "FileReader";

/* [@bs.send] external readAsBinaryString: (fileReader, file) => unit = ""; */

[@bs.send] external readAsDataURL: (fileReader, file) => unit = "";

[@bs.send] external readAsArrayBuffer: (fileReader, file) => unit = "";

[@bs.send] external readAsText: (fileReader, file) => unit = "";

external convertResultToString: resultType => string = "%identity";

external convertResultToArrayBuffer: resultType => Js.Typed_array.array_buffer =
  "%identity";

external convertResultToBlob: resultType => Js.Typed_array.array_buffer =
  "%identity";

external convertResultToArrayBuffer: resultType => Js.Typed_array.ArrayBuffer.t =
  "%identity";

let onload: (fileReader, resultType => unit) => unit = [%bs.raw
  {|
      function (reader,handleFunc) {
          reader.onload = function() {
              handleFunc(this.result)
          }
      }
  |}
];
let buildFileChunkFromDataArray = ((blockCount, chunkSize), fileHash, file) => {
  let resultArray = [||];
  let fileSize = file##size;

  for (x in 0 to blockCount - 1) {
    let start = x * chunkSize;
    let end_ = Js.Math.min_int(fileSize, start + chunkSize);
    let formData = FormData.createFormData();

    FormData.append(
      formData,
      "file",
      Blob.slice(start, end_, file |> FileType.convertFileToBlob),
    );
    FormData.append(formData, "index", x);
    FormData.append(formData, "hash", fileHash);

    resultArray |> Js.Array.push(formData) |> ignore;
  };

  resultArray;
};