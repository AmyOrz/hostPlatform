open Antd;

open UserCurrentRepoStore;

module Method = {
  let buildContentTextWithTextArray = textArray =>
    textArray
    |> Js.Array.mapi((itemObj, i) => {
         let text = itemObj##text;

         itemObj##hasColor ?
           <span className="text-color" key={i |> string_of_int}>
             {DomHelper.textEl({j|$text|j})}
           </span> :
           <span key={i |> string_of_int}>
             {DomHelper.textEl({j|$text|j})}
           </span>;
       });
};

let component = ReasonReact.statelessComponent("ProjectOverView");

let make =
    (
      ~store: AppStore.appState,
      ~dispatch,
      ~currentRepo,
      ~languageType,
      _children,
    ) => {
  ...component,
  render: _self =>
    <div className="wonder-project-overView">
      <div className="project-nav">
        <div className="nav-item">
          <div className="item-type">
            {DomHelper.textEl({j|项目信息|j})}
          </div>
          <div className="item-content">
            <div className="content-team">
              <div className="team-header">
                {DomHelper.textEl({j|创建时间|j})}
              </div>
              <div className="team-content">
                {DomHelper.textEl(currentRepo.createDate)}
              </div>
            </div>
            <div className="content-team">
              <div className="team-header">
                {DomHelper.textEl({j|访问权限|j})}
              </div>
              <div className="team-content">
                {
                  DomHelper.textEl(
                    ValueUtils.isValueEqual(currentRepo.access, 1) ?
                      {j|私有项目|j} : {j|公开项目|j},
                  )
                }
              </div>
            </div>
            <div className="content-team">
              <div className="team-header">
                {DomHelper.textEl({j|文件大小|j})}
              </div>
              <div className="team-content">
                {
                  DomHelper.textEl(
                    (currentRepo.size |> string_of_float) ++ "M",
                  )
                }
              </div>
            </div>
          </div>
        </div>
      </div>
      <div className="project-content">
        <div className="content-title">
          {DomHelper.textEl({j|操作步骤 : |j})}
        </div>
        <div className="content-text">
          {
            ReasonReact.array(
              Method.buildContentTextWithTextArray([|
                {"text": {j|1. 点击|j}, "hasColor": false},
                {"text": {j|编辑器打开|j}, "hasColor": true},
                {
                  "text": {j|， 进入编辑器编辑场景。|j},
                  "hasColor": false,
                },
              |]),
            )
          }
        </div>
        <div className="content-text">
          {
            ReasonReact.array(
              Method.buildContentTextWithTextArray([|
                {
                  "text": {j|2. 编辑场景完成以后点击|j},
                  "hasColor": false,
                },
                {"text": {j| 文件 |j}, "hasColor": true},
                {"text": {j|->|j}, "hasColor": true},
                {"text": {j| 保存场景 |j}, "hasColor": true},
                {
                  "text": {j|手动保存场景， 方便下次打开时是最新的场景， 我们为您设置为每间隔5分钟默认保存一次。|j},
                  "hasColor": false,
                },
              |]),
            )
          }
        </div>
        <div className="content-text">
          {
            ReasonReact.array(
              Method.buildContentTextWithTextArray([|
                {
                  "text": {j|3. 您可以在编辑器中点击|j},
                  "hasColor": false,
                },
                {"text": {j| 发布 |j}, "hasColor": true},
                {"text": {j|->|j}, "hasColor": true},
                {"text": {j| 托管平台 |j}, "hasColor": true},
                {
                  "text": {j|进行项目发布， 发布以后大家可以在|j},
                  "hasColor": false,
                },
                {"text": {j| 公开项目 |j}, "hasColor": true},
                {
                  "text": {j|中看到您发布的项目。|j},
                  "hasColor": false,
                },
              |]),
            )
          }
        </div>
        <div className="content-text">
          {
            ReasonReact.array(
              Method.buildContentTextWithTextArray([|
                {
                  "text": {j|4. 您发布以后可以在本页面点击|j},
                  "hasColor": false,
                },
                {"text": {j| 运行 |j}, "hasColor": true},
                {
                  "text": {j|来运行查看您的项目。|j},
                  "hasColor": false,
                },
              |]),
            )
          }
        </div>
        <div className="content-title">
          {DomHelper.textEl({j|注意事项 ：|j})}
        </div>
        <div className="content-tip">
          {
            DomHelper.textEl(
              {j|使用编辑器打开别人项目时， 无法进行保存和发布。|j},
            )
          }
        </div>
        <div className="content-tip">
          {
            DomHelper.textEl(
              {j|可以使用Fork 把别人的项目克隆到自己目录下，然后使用编辑器打开，进行保存和发布。|j},
            )
          }
        </div>
      </div>
    </div>,
};