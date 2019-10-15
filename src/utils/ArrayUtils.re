let create = () => [||];

let range = (a: int, b: int) => {
  let result = WonderCommonlib.ArrayService.createEmpty();
  for (i in a to b) {
    Js.Array.push(i, result) |> ignore;
  };
  result;
};

let unsafeGetFirst = arr =>
  WonderCommonlib.ArrayService.unsafeGet(arr, 0)
  |> WonderLog.Contract.ensureCheck(
       r =>
         WonderLog.(
           Contract.(
             Operators.(
               test(
                 Log.buildAssertMessage(
                   ~expect={j|array[0] element exist|j},
                   ~actual={j|not|j},
                 ),
                 () =>
                 r |> assertNullableExist
               )
             )
           )
         ),
       SystemConfig.getIsDebug(),
     );

let getFirst = arr =>
  WonderCommonlib.ArrayService.unsafeGet(arr, 0) |> Js.toOption;

let unsafeGetLast = arr =>
  arr
  |> Js.Array.length
  |> (len => len - 1)
  |> WonderCommonlib.ArrayService.unsafeGet(arr)
  |> WonderLog.Contract.ensureCheck(
       r =>
         WonderLog.(
           Contract.(
             Operators.(
               test(
                 Log.buildAssertMessage(
                   ~expect={j|array[0] element exist|j},
                   ~actual={j|not|j},
                 ),
                 () =>
                 r |> assertNullableExist
               )
             )
           )
         ),
       SystemConfig.getIsDebug(),
     );

let getLast = arr =>
  arr
  |> Js.Array.length
  |> (len => len - 1)
  |> WonderCommonlib.ArrayService.unsafeGet(arr)
  |> Js.toOption;

let removeLast = arr => {
  WonderLog.Contract.requireCheck(
    () =>
      WonderLog.(
        Contract.(
          Operators.(
            test(
              Log.buildAssertMessage(
                ~expect={j|array length should >= 1|j},
                ~actual={j|not|j},
              ),
              () =>
              arr |> Js.Array.length >= 1
            )
          )
        )
      ),
    SystemConfig.getIsDebug(),
  );
  arr |> Js.Array.pop |> Js.Option.getExn;
};

let removeFirst = arr => {
  WonderLog.Contract.requireCheck(
    () =>
      WonderLog.(
        Contract.(
          Operators.(
            test(
              Log.buildAssertMessage(
                ~expect={j|array length should >= 1|j},
                ~actual={j|not|j},
              ),
              () =>
              arr |> Js.Array.length >= 1
            )
          )
        )
      ),
    SystemConfig.getIsDebug(),
  );
  arr |> Js.Array.shift |> Js.Option.getExn;
};

let unsafeGetNth = (index, arr) =>
  WonderCommonlib.ArrayService.unsafeGet(arr, index)
  |> WonderLog.Contract.ensureCheck(
       r =>
         WonderLog.(
           Contract.(
             Operators.(
               test(
                 Log.buildAssertMessage(
                   ~expect={j|array[$index] element exist|j},
                   ~actual={j|not|j},
                 ),
                 () =>
                 r |> assertNullableExist
               )
             )
           )
         ),
       SystemConfig.getIsDebug(),
     );

let hasItem = arr => arr |> Js.Array.length > 0 ? true : false;

let unshift = (item, arr) => {
  arr |> Js.Array.unshift(item) |> ignore;
  arr;
};

let push = (item, arr) => {
  arr |> Js.Array.push(item) |> ignore;
  arr;
};

let pushMany = (itemArr, arr) =>
  itemArr
  |> WonderCommonlib.ArrayService.reduceOneParam(
       (. arr, item) => arr |> push(item),
       arr,
     );

let hasItemByFunc = (func, arr) => arr |> Js.Array.filter(func) |> hasItem;