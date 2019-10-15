type file = {
  .
  "name": string,
  "size": int,
  "type": string,
};

external convertFileJsObjectToFile: Js.t({..}) => file = "%identity";

external convertFileToBlob: file => Blob.blob = "%identity";