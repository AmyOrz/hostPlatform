let convertArrayBufferToBase64: Js.Typed_array.ArrayBuffer.t => string = [%raw
  buffer => {|
    var binary = '';
    var bytes = new Uint8Array(buffer);
    for (var len = bytes.byteLength, i = 0; i < len; i++) {
        binary += String.fromCharCode(bytes[i]);
    }
    return window.btoa(binary);
  |}
];

let convertBase64ToUint8Array: string => Js.Typed_array.Uint8Array.t = [%raw
  dataURI => {|
  var BASE64_MARKER = ';base64,';

  var base64Index = dataURI.indexOf(BASE64_MARKER) + BASE64_MARKER.length;
  var base64 = dataURI.substring(base64Index);
  var raw = window.atob(base64);
  var rawLength = raw.length;
  var array = new Uint8Array(new ArrayBuffer(rawLength));

  for(var i = 0; i < rawLength; i++) {
    array[i] = raw.charCodeAt(i);
  }
  return array;
  |}
];

let convertStrToTypeArray: string => Js.Typed_array.ArrayBuffer.t = [%raw
  str => {|
      var buf = new ArrayBuffer(str.length*2);
      var bufView = new Uint16Array(buf);
      for (var i=0, strLen=str.length; i<strLen; i++) {
        bufView[i] = str.charCodeAt(i);
      }
      return buf;
  |}
];