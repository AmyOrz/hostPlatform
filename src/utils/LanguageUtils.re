open LanguageType;

open LanguageDataType;

let getLanguageType = language =>
  switch (language) {
  | "ZH" => ZH
  | "EN" => EN
  | _ => EN
  };
let changeLanguage = languageType =>
  switch (languageType) {
  | EN =>
    LocalStorage.setValue("language", "ZH");
    DomHelper.locationReload(.);
  | ZH =>
    LocalStorage.setValue("language", "EN");
    DomHelper.locationReload(.);
  };

let _getLanguageDataByType = (languageDataArray, itemName, type_) => {
  let {language} =
    languageDataArray
    |> Js.Array.filter(({title}) => title === itemName)
    |> ArrayUtils.unsafeGetFirst;

  switch (type_) {
  | EN => language.en
  | ZH => language.zh
  };
};

let getHeaderLanguageDataByType = (itemName, type_) =>
  LanguageData.header_language_array
  ->(_getLanguageDataByType(itemName, type_));

let getHomePageLanguageDataByType = (itemName, type_) =>
  LanguageData.homePage_language_array
  ->(_getLanguageDataByType(itemName, type_));