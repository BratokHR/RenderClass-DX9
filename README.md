RenderClass
===========
**RenderClass** - это класс который служит "мостом" между кодером и DirectX. Он сам обрабатывает потерю устройства, пересоздает нужные объекты для рендеринга. Вам всего лишь нужно указать функцию инициализации и прописать в нужных местах обращения к классу.
### Подключение
Вам нужно разобраться с принципом подключения класса и избавиться от головной боли с перезагрузкой шрифтов, с нудной работой с вершинным буфером.
Вот шаблон проекта.
``` c++
#include "cRender.h"

cRender render;

HRESULT APIENTRY hReset( LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters )
{
    // [*]
    render.LostDevice();

    HRESULT hRet = pReset( pDevice, pPresentationParameters );

    // [*]
    render.ResetDevice();

    return hRet;
}

HRESULT APIENTRY hEndScene( LPDIRECT3DDEVICE9 pDevice )
{
    // [*] Проверка устройства
    render.CheckObjects( pDevice );

    // Рисование

    HRESULT hRet = pEndScene( pDevice );

    // [* если используете текстуры] Очистка текстур
    render.ClearTextures();

    return hRet;
}

void InitObjects()
{
    // Создание необходимых шрифтов, линий, текстур и т.д.
}

void HookDirect3D()
{
    // Указываем функцию создания объектов
    // [*] Будет вызываться автоматически классом
    render.InitFunction = InitObjects;

    // Хук директа
}
```
> Все обязательные действия помечены **\***.

Больше информации найдете <https://github.com/BratokHR/RenderClass-DX9/wiki>
