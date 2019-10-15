let await: (unit => Js.Promise.t(string)) => unit = [%bs.raw
  {|
    async function(promiseFunc) {
      var a = await promiseFunc();
      return a;
    }
  |}
];