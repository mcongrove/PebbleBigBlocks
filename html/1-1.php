<!DOCTYPE html>
<html>
<head>
<title>"Big Blocks" Configuration</title>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
<script src="http://wearewearable.com/pconfig/pconfig.js" type="text/javascript"></script>
<style type="text/css">
html,body,div,span,applet,object,iframe,h1,h2,h3,h4,h5,h6,p,blockquote,pre,a,abbr,acronym,address,big,cite,code,del,dfn,em,font,img,ins,kbd,q,s,samp,small,strike,strong,sub,sup,tt,var,b,u,i,center,dl,dt,dd,ol,ul,li,fieldset,form,label,legend,table,caption,tbody,tfoot,thead,tr,th,td{margin:0;padding:0;outline:0;border:0;background:transparent;vertical-align:baseline;font-size:100%}body{background:#FFF;line-height:1}ol,ul{list-style:none}blockquote,q{quotes:none}:focus{outline:0}ins{text-decoration:none}del{text-decoration:line-through}table{border-spacing:0;border-collapse:collapse}a{text-decoration:none}a:hover{text-decoration:underline}.clear{clear:both}.container{margin:0;padding:0 0 20px;width:100%}h2{padding:0 2%;width:96%;height:45px;background:#000;color:#FFF;letter-spacing:.07em;font-weight:300;font-size:14px;font-family:"HelveticaNeue-Light","Helvetica Neue Light","Helvetica Neue",Helvetica,Arial,"Lucida Grande",sans-serif;line-height:45px}h3{padding:0;margin:0 0 10px;width:100%;height:20px;color:#000;letter-spacing:.07em;font-weight:300;font-size:14px;font-family:"HelveticaNeue-Light","Helvetica Neue Light","Helvetica Neue",Helvetica,Arial,"Lucida Grande",sans-serif;line-height:20px}h4{margin:10px 0 30px;padding:0 2%;width:96%;height:20px;color:#000;text-align:center;font-weight:700;font-size:11px;font-family:"HelveticaNeue-Bold","Helvetica Neue Bold","Helvetica Neue",Helvetica,Arial,"Lucida Grande",sans-serif;line-height:20px}h5{margin:5px 0 0;padding:0 2%;width:96%;height:11px;color:#333;text-align:center;font-weight:300;font-size:9px;font-family:"HelveticaNeue-Light","Helvetica Neue Light","Helvetica Neue",Helvetica,Arial,"Lucida Grande",sans-serif;line-height:11px}#themes{width:100%}#themes ul{list-style-type:none}#themes ul li{float:left;padding:10px 5% 0;width:40%;background:#EEE;text-align:center}#themes ul li.selected{background:#333}#themes ul li:first-child{padding:10px 5% 0}#themes ul li img{margin:10px auto;max-width:144px;pointer-events:none}#themes ul li div{margin:0 auto 10px;width:20px;height:20px;-webkit-border-radius:10px;-moz-border-radius:10px;border-radius:10px;background:#FFF;pointer-events:none}#themes ul li.selected div{background:#000}#time{width:100%;padding:0 0 10px}#time ul{width:100%}#time ul li{float:left;width:92%;margin:10px 4%}#time ul li select{width:100%;border:1px solid gray;color:#000;background:#EEE;font-weight:700;font-size:14px;font-family:"HelveticaNeue-Medium","Helvetica Neue Medium","Helvetica Neue",Helvetica,Arial,"Lucida Grande",sans-serif}#buttons{width:100%;height:50px}#buttons ul{list-style-type:none}#buttons ul li{float:left;width:50%;height:50px;background:#000;color:#FFF;text-align:center;letter-spacing:.08em;font-weight:700;font-size:14px;font-family:"HelveticaNeue-Medium","Helvetica Neue Medium","Helvetica Neue",Helvetica,Arial,"Lucida Grande",sans-serif;line-height:50px;cursor:pointer}#buttons ul li#cancel{width:35%;background:#C33;color:#7A0000}#buttons ul li#save{width:65%;background:#3C3;color:#007A00}
</style>
</head>

<body>
	<div class="container">
		<div id="themes">
			<h2>
				Select a theme:
			</h2>
			
			<ul>
				<li id="theme-dark"<?php echo $_GET['theme'] == 'dark' || !$_GET['theme'] ? ' class="selected"' : ''; ?>>
					<img src="big-blocks-dark.png" />
					
					<div />
				</li>
				<li id="theme-light"<?php echo $_GET['theme'] == 'light' ? ' class="selected"' : ''; ?>>
					<img src="big-blocks-light.png" />
					
					<div />
				</li>
			</ul>
			
			<input type="hidden" id="theme" data-type="string" value="<?php echo isset($_GET['theme']) && $_GET['theme'] != 'undefined' ? $_GET['theme'] : 'dark'; ?>" />
			
			<br class="clear" />
		</div>
		
		<div id="time">
			<h2>
				Display options:
			</h2>
			
			<ul>
				<li>
					<h3>
						Select a time display option:
					</h3>
					
					<select id="time_display" data-type="int">
						<?php
						
							$options = array(
								0 => 'Only blocks',
								1 => 'Show hour',
								2 => 'Show minutes',
								3 => 'Show full time'
							);
							
							foreach($options as $key => $option)
							{
								$selected = '';
								
								if($_GET['time'] == $key)
								{
									$selected = ' selected="selected"';
								}
								
								echo '<option value="' . $key . '"' . $selected . '>' . $option . '</option>';
							}
						
						?>
					</select>
				</li>
			</ul>
			
			<br class="clear" />
		</div>
		
		<div id="buttons">
			<ul>
				<li id="cancel">
					Cancel
				</li>
				<li id="save">
					Apply Changes
				</li>
			</ul>
			
			<br class="clear" />
			
			<h4>
				Please allow a few moments for your changes to take affect.
			</h4>
			
			<h5>
				www.WeAreWearable.com
			</h5>
			
			<h5>
				"Big Blocks" watchface for Pebble, developed by Matthew Congrove
			</h5>
			
			<h5>
				www.MatthewCongrove.com
			</h5>
		</div>
	</div>
	
	<script type="text/javascript">
		PConfig.init();
		
		function handleThemeSelection(_event) {
			switch(_event.target.id) {
				case "theme-dark":
					document.getElementById("theme-dark").className = "selected";
					document.getElementById("theme-light").className = "";
					
					document.getElementById("theme").value = "dark";
					break;
				case "theme-light":
					document.getElementById("theme-light").className = "selected";
					document.getElementById("theme-dark").className = "";
					
					document.getElementById("theme").value = "light";
					break;
			}
		}
		
		document.getElementById("theme-dark").addEventListener("click", handleThemeSelection);
		document.getElementById("theme-light").addEventListener("click", handleThemeSelection);
		document.getElementById("cancel").addEventListener("click", PConfig.onCancel);
		document.getElementById("save").addEventListener("click", PConfig.onSubmit);
    </script>
</body>
</html>