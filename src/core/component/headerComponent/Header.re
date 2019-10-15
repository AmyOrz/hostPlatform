open LanguageType;

open WonderBsJson;

open WonderBsMost;

open AppStore;

module Method = {};

let component = ReasonReact.statelessComponent("Header");

let render = (store, dispatch, languageType, _self) =>
  <div className="wonder-header">
    <div className="header-logo" onClick={_e => DomHelper.locationHref("/")}>
      <img src="/img/logo.png" className="logo-img" />
      <div className="logo-text">
        {DomHelper.textEl("Wonder Technology")}
      </div>
    </div>
    <div className="header-white" />
    <div className="header-nav">
      <div className="nav-item">
        <img src="/img/project.png" />
        <a href="/publicProject"> {DomHelper.textEl({j|公开项目|j})} </a>
      </div>
      <div className="nav-item">
        <img src="/img/vip.png" />
        <a href="/docs/detail"> {DomHelper.textEl({j|收费|j})} </a>
      </div>
      <div className="nav-item">
        <img src="/img/forum.png" />
        <a
          href="https://forum.wonder-3d.com/Wonder-Technology" target="_blank">
          {DomHelper.textEl({j|论坛|j})}
        </a>
      </div>
      <div className="nav-item">
        <img src="/img/home.png" />
        <a href="https://www.wonder-3d.com/Wonder-Technology" target="_blank">
          {DomHelper.textEl({j|官网|j})}
        </a>
      </div>
      <div className="nav-item nav-profile">
        <img
          src={
            SystemConfig.getServerPath()
            ++ StoreUtils.getUserProfilePath(store)
          }
        />
      </div>
    </div>
  </div>;
/* <div
     className="nav-item"
     onClick={_e => LanguageUtils.changeLanguage(languageType)}>
     <div className="item-language">
       {
         switch (languageType) {
         | EN => DomHelper.textEl({j|中文|j})
         | ZH => DomHelper.textEl({j|English|j})
         }
       }
     </div>
   </div> */

let make = (~store: AppStore.appState, ~dispatch, ~languageType, _children) => {
  ...component,
  render: self => render(store, dispatch, languageType, self),
};