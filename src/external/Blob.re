type blob;

external convertToString: blob => string = "%identity";

let newBlobFromArrayBuffer: (Js.Typed_array.array_buffer, string) => blob = [%raw
  (arrayBuffer, blobType) => {|
    return new Blob([arrayBuffer], {type: blobType})
  |}
];

let slice: (int, int, blob) => blob = [%raw
  (start, end_, blob) => {|
     return blob.slice( start, end_)
    |}
];

let createObjectURL: blob => string = [%raw
  blob => {|
     return URL.createObjectURL( blob )
    |}
];

let revokeObjectURL = [%raw
  blob => {|
     URL.revokeObjectURL( blob );
    |}
];

let convertToBase64 = [%raw
  blob => {|
    return new Buffer( blob, 'binary' ).toString('base64');
    |}
];