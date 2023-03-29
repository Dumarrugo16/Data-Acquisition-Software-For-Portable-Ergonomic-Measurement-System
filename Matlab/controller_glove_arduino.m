function varargout = controller_glove_arduino(varargin)
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @controller_glove_arduino_OpeningFcn, ...
                   'gui_OutputFcn',  @controller_glove_arduino_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1}), gui_State.gui_Callback = str2func(varargin{1}); end
if nargout, [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else, gui_mainfcn(gui_State, varargin{:}); end

function controller_glove_arduino_OpeningFcn(hObject, eventdata, handles, varargin)
handles.output = hObject; axes_handles = axes('unit','normalized','position',[0 0 1 1]); % spans whole gui.
background = imread('background.jpeg'); imagesc(background); % import the background image and show on the axes.
set(axes_handles,'handlevisibility','off','visible','off'); % prevent plotting over the background - turn axis off.
uistack(axes_handles, 'bottom'); guidata(hObject, handles); % making sure background is behind all other ui-controls.

function varargout = controller_glove_arduino_OutputFcn(hObject, eventdata, handles) 
varargout{1} = handles.output;

function drawing_obstacles_Callback(hObject, eventdata, handles), cla(handles.screen_videogame,'reset');
global center_x shift_x ix sx iy sy, ix = 0; sx = 100; iy = 0; sy = 100; center_x = 17; counter = 0;
global player centers shifts blocks quantity_blocks; if quantity_blocks > 70, quantity_blocks = 70; end;
limits_x = [ix sx]; set(handles.screen_videogame,'xlim',limits_x); shift_x = 7; shift_y = sy-0.5*shift_x;
limits_y = [iy sy]; set(handles.screen_videogame,'ylim',limits_y); axes(handles.screen_videogame);
set(handles.screen_videogame,'xticklabel',[]); set(handles.screen_videogame,'yticklabel',[]); blocks = [];
red = rand(quantity_blocks,1); green = rand(quantity_blocks,1); blue = rand(quantity_blocks,1);
while counter < quantity_blocks, x = counter + 1; if x == 1, initial_x = [center_x shift_x]; end;
    centers(x) = center_x; shifts(x) = shift_y;
    blocks(x) = line([center_x-shift_x center_x+shift_x],[shift_y shift_y],'linewidth',shift_x,'color',[red(x),green(x),blue(x)]);
    counter = counter + 1; center_x = initial_x(1) + center_x; 
    if shift_y < 0.5 * sy, counter = quantity_blocks; delete(blocks); end;
    if center_x > sx - 0.5 * initial_x(1), center_x = initial_x(1); shift_y = shift_y - 0.5*initial_x(2); end; end
player = line([0.5*sx-1.1*shift_x 0.5*sx+1.1*shift_x],[0.5*shift_x 0.5*shift_x],'linewidth',shift_x,'color',[0.0,0.0,0.0]);

function drawing_destroyer_Callback(hObject, eventdata, handles), global balloon position_balloon
global sx sy diameter_balloon; position_balloon = [0.5 * sx, 0.25 * sy]; axes(handles.screen_videogame);
balloon = line(position_balloon(1),position_balloon(2),'marker','.','markersize',diameter_balloon,'color','red');
global fails j available points; fails = []; j = 1; available = true; points = 0;

function start_game_Callback(hObject, eventdata, handles), global diameter_balloon balloon velocity_balloon position_balloon
global shift_x sx sy ix iy player place quantity_blocks points centers shifts blocks fails j available starting center_x flex
global bending voltage; mismatch = 0.1 * diameter_balloon; axes(handles.screen_videogame); place = sx; message = 'game over!'; 
global potentiometer nano graph x y time flex_voltage; index = length(potentiometer); title = 'finished game'; starting = true;
while starting == true
    
    % ------------------------------------------------------------------------------------------------ %
    
    potentiometer = readVoltage(nano,'A0'); pause(0.0001); x = get(graph,'xdata'); y = get(graph,'ydata');
    x = [x time]; y = [y potentiometer]; set(graph,'xdata',x,'ydata',y); time = time + 1; % pause(1); end
    set(handles.time_X,'string',num2str(time*0.01)); set(handles.voltage_Y,'string',num2str(potentiometer));
    
    % ------------------------------------------------------------------------------------------------ %
    
    bending = 7.78 + 1.43 * potentiometer - 0.86 * potentiometer^2 + 0.98 * potentiometer ^ 3; pause(0.01);
    voltage = [voltage potentiometer]; flex = [flex bending]; set(flex_voltage,'xdata',voltage,'ydata',flex);
    set(handles.voltage_X,'string',num2str(potentiometer)); set(handles.bending_Y,'string',num2str(bending));
    
    % ------------------------------------------------------------------------------------------------ %
    
    % SIMULACIÓN DEL MOVIMIENTO DE LA BOLA Y COLISIÓN CON EL BLOQUE DEL JUGADOR.
    if position_balloon(1) < ix + mismatch || position_balloon(1) > sx - mismatch
         velocity_balloon(1) = - velocity_balloon(1); end
    if position_balloon(2) > sy - mismatch, velocity_balloon(2) = - velocity_balloon(2); end
    if position_balloon(2) < iy + mismatch + 1.11
        if abs(position_balloon(1) - 0.5 * place) < shift_x * 1.33
            velocity_balloon(2) = - 2.22*rand(1,1)*velocity_balloon(2);
            if velocity_balloon(2) <= 0.5, velocity_balloon(1) = velocity_balloon(1) * 1.33; end
        else, game_over = warndlg(message,title); pause(7.77); end; end
    % DESTRUCCIÓN DE BLOQUES AL COLISIONAR CON LA PELOTA (SUB-ETAPA).
    for i = 1:quantity_blocks 
        for k = 1 : length(fails)
            if i == fails(k), available = false; end; end
        if abs(position_balloon(1) - centers(i)) < shift_x && available == true
            if abs(position_balloon(2) - shifts(i) - 0.5 * shift_x) < 0.11 * shift_x;
                fails(j) = i; j = j + 1; delete(blocks(i)); points = points + 1; 
                velocity_balloon(2) = - 0.777*velocity_balloon(2);
                if velocity_balloon(1) <= 0.5, velocity_balloon(1) = velocity_balloon(1) * 1.33; end
                if velocity_balloon(2) <= 0.5, velocity_balloon(2) = velocity_balloon(2) * 1.33; end
            end
        end; available = true;
    end
    % RETOMAMOS EL MOVIMIENTO DE LA PELOTA Y POSICIONAMIENTO DEL PLAYER.
    position_balloon = position_balloon + velocity_balloon;
    set(handles.points_game,'string',sprintf('%d',points));
    set(balloon,'xdata',position_balloon(1),'ydata',position_balloon(2));
    set(player,'xdata',[0.5*place-1.1*shift_x 0.5*place+1.1*shift_x]); pause(.02);  
    if potentiometer >= 0.0 && potentiometer <= 2.5, place = place + potentiometer - 2.5; end
    if 0.5 * place < 1.1 * shift_x, place = 2.2 * shift_x; end
    if potentiometer > 2.5 && potentiometer <= 5.0, place = place + potentiometer - 2.5; end
    if 0.5 * place > sx - 1.1 * shift_x, place = 2 * sx - 2.2 * shift_x; end
end

function diameter_balloon_Callback(hObject, eventdata, handles), global diameter_balloon
diameter_balloon = str2double(get(handles.diameter_balloon,'string'));

function diameter_balloon_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white'); end

function velocity_balloon_Callback(hObject, eventdata, handles), global velocity_balloon
velocity_balloon = [str2double(get(handles.velocity_balloon,'string')) * 0.777 ... 
                    str2double(get(handles.velocity_balloon,'string')) * 1.111];
                
function velocity_balloon_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white'); end

function quantity_blocks_Callback(hObject, eventdata, handles), global quantity_blocks
quantity_blocks = str2double(get(handles.quantity_blocks,'string'));

function quantity_blocks_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white'); end

function points_game_Callback(hObject, eventdata, handles)
function points_game_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white'); end

function extract_dataset_Callback(hObject, eventdata, handles)
[filename path] = uigetfile({'*.xlsx'},'abrir xlsx'); set(handles.path_excel,'string',[path, filename]);
excel = get(handles.path_excel,'string'); data = xlsread(excel); time = data(:,1); bending = data(:,2); voltage = data(:,3);
axes(handles.graph_flex_voltage); plot(bending,voltage,'green','linewidth',3); xlabel('bending [m]'); ylabel('voltage [v]');
axes(handles.graph_time_flex); plot(time,bending,'blue','linewidth',3); xlabel('time [s]'); ylabel('bending [m]');

function path_excel_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white'); end

function data_acquisition_Callback(hObject, eventdata, handles), global potentiometer graph x y
global nano selected bending time; if ~isempty(instrfind), fclose(instrfind); delete(instrfind); end;
selected = true; nano = arduino('COM4','Nano3'); configurePin(nano,'A0','AnalogInput'); 
% set(handles.graph_time_flex,'xlim',[0 2000]); set(handles.graph_time_flex,'ylim',[0 5]);
global voltage flex; axes(handles.graph_time_flex); graph = line(nan,nan,'color','blue'); time = 0;
global flex_voltage;  axes(handles.graph_flex_voltage); flex_voltage = line(nan,nan,'color','green'); 
while selected == true
    potentiometer = readVoltage(nano,'A0'); pause(0.01); x = get(graph,'xdata'); y = get(graph,'ydata');
    x = [x time]; y = [y potentiometer]; set(graph,'xdata',x,'ydata',y); time = time + 1; 
    set(handles.time_X,'string',num2str(time*0.01)); set(handles.voltage_Y,'string',num2str(potentiometer));
    
    % VERSIÓN BETA (NO CALIBRADA) PARA GRAFICAR ANGULOS DE DEFLEXIÓN DEL DEDO VS VOLTAGE A TRAVÉS DEL FLEX.
    bending = 7.78 + 1.43 * potentiometer - 0.86 * potentiometer^2 + 0.98 * potentiometer ^ 3; pause(0.01);
    voltage = [voltage potentiometer]; flex = [flex bending]; set(flex_voltage,'xdata',voltage,'ydata',flex);
    set(handles.voltage_X,'string',num2str(potentiometer)); set(handles.bending_Y,'string',num2str(bending));
end

function time_X_Callback(hObject, eventdata, handles)
function time_X_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white'); end

function voltage_Y_Callback(hObject, eventdata, handles)
function voltage_Y_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white'); end

function voltage_X_Callback(hObject, eventdata, handles)
function voltage_X_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white'); end

function bending_Y_Callback(hObject, eventdata, handles)
function bending_Y_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white'); end
