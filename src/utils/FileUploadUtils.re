open WonderBsMost;

let getFileHashValue = (fileName, stream) =>
  stream
  |> Most.flatMap(file =>
       Most.fromPromise(
         Js.Promise.make((~resolve, ~reject) => {
           let reader = FileReader.createFileReader();

           FileReader.onload(reader, result => resolve(. result));

           FileReader.readAsArrayBuffer(reader, file);
         }),
       )
     )
  |> Most.flatMap(fileResult =>
       Crypto.getHashValue(
         fileResult |> FileReader.convertResultToArrayBuffer,
       )
       |> Most.fromPromise
       |> Most.flatMap(fileHash =>
            Crypto.getHashValue(fileName |> ArrayBuffer.convertStrToTypeArray)
            |> Most.fromPromise
            |> Most.map(nameHash =>
                 (fileHash |> StringUtils.sliceHalfString)
                 ++ (nameHash |> StringUtils.sliceHalfString)
               )
          )
     );