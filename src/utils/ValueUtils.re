let isValueValid = [%raw
  value => {|
   return value !== undefined && value !== null;
  |}
];

let isValueEqual = (sourceValue, targetValue) => sourceValue === targetValue;