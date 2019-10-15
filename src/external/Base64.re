let convertStrToBase64: string => string = [%raw
  string => {|
   window.btoa(string)
  |}
];

let convertBase64ToStr: string => string = [%raw
  string => {|
   window.atob(string)
  |}
];