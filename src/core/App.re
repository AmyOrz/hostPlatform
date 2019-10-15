open WonderBsJson;

open JustgageReasonCookie;

module RouterConfig = {
  type route =
    | UserActive
    | Sign
    | PublicProject
    | TargetProject
    | HomePage;

  let routeFromUrl = (url: ReasonReact.Router.url) =>
    switch (url.path) {
    | ["userActive"] => UserActive
    | ["sign"] => Sign
    | ["targetProject"] => TargetProject
    | ["publicProject"] => PublicProject
    | _ => HomePage
    };
  let routeToUrl = (route: route) =>
    switch (route) {
    | UserActive => "/userActive"
    | Sign => "/sign"
    | PublicProject => "/publicProject"
    | TargetProject => "/targetProject"
    | HomePage => "/"
    };
};

module Router = ReRoute.CreateRouter(RouterConfig);

let component = ReasonReact.statelessComponent("App");

let make = (~state as store: AppStore.appState, ~dispatch, _children) => {
  ...component,
  render: _self =>
    <Router.Container>
      ...{
           (~currentRoute) => {
             let languageType =
               LanguageUtils.getLanguageType(Window.window##wonderLanguage);

             <div className="wonder-app">
               {
                 switch (Cookie.getAsJson("userData")) {
                 | None =>
                   switch (currentRoute) {
                   | RouterConfig.UserActive => <UserActive />
                   | RouterConfig.Sign => <UserSignComponent languageType />
                   | RouterConfig.TargetProject
                   | RouterConfig.PublicProject
                   | RouterConfig.HomePage =>
                     DomHelper.locationHref("/sign");
                     ReasonReact.null;
                   }

                 | Some(userData) =>
                   let userDataObj = JsonType.convertToJsObj(userData);
                   let userId = userDataObj##userId;

                   switch (currentRoute) {
                   | RouterConfig.Sign
                   | RouterConfig.UserActive =>
                     DomHelper.locationHref("/");
                     ReasonReact.null;
                   | RouterConfig.HomePage =>
                     <HomePageComponent store dispatch languageType userId />
                   | RouterConfig.TargetProject =>
                     <ProjectComponent store dispatch userId languageType />
                   | RouterConfig.PublicProject =>
                     <PublishProjectComponent store dispatch userId languageType />
                   };
                 }
               }
             </div>;
           }
         }
    </Router.Container>,
};