/* open LanguageParseType;

let convertDataToRecord = jsonData =>
  WonderBsJson.Json.(
    Decode.(
      jsonData
      |> Js.Json.parseExn
      |> array(json =>
           {
             title: json |> field("title", string),
             language:
               json
               |> field("language", json =>
                    {
                      us: json |> field("US", string),
                      zh: json |> field("ZH", string),
                    }
                  ),
           }
         )
    )
  );
  let getLanguage = () =>
  Language.language_data
  |> convertDataToRecord; */